#define _GNU_SOURCE

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "shapes/shapes.h"
#include "template.glsl.h"

// TODO group modifiers are transfered even for groups without a paramterer.
// TODO child position relative to parent shape
// object that are only visible after the first bounce?
//  -> player camera can be inside the player character but only visible
//     in mirrors and through portals and such

Scene *scene_new() {
	Scene *s = xmalloc_zero(sizeof(Scene));

	s->eps = 0.01f;
	s->max_dist = 64.0f;
	s->rm_iters = 256;
	s->main_iters = 8;

	s->root        = NULL;
	s->flat_prims  = NULL;
	s->flat_groups = NULL;

	s->portals = xmalloc(sizeof(Portal) * MAX_PORTALS_PER_SCENE);
	for (int i = 0; i < MAX_PORTALS_PER_SCENE; i++)
		s->portals[i] = NULL;

	// default camera settings
	s->cam.position = (Vector3){ 0.0f, 2.0f, 0.0f };
	s->cam.target   = (Vector3){ 0.0f, -2.0f, 1.0f };
	s->cam.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
	s->cam.fovy     = 65.0f;

	SetCameraMode(s->cam, CAMERA_FIRST_PERSON);

	s->tree_changed   = true;
	s->primt_changed  = true;
	s->group_changed  = true;
	s->portal_changed = true;

	return s;
}

char* scene_create_sdf(Scene *s) {
	char* sdf = xmalloc_zero(8192);

	int s_pos = 0;
	int g_pos = 0;
	void sdf_gen(Shape *pos) {
		char *c;
		switch (pos->type) {
			case stPRIMITIVE:
				switch (pos->p.type) {
					case ptSPHERE:
						asprintf(&c, "clrd(sC(%d),d2Sphere(pos,sP(%d),sR1(%d)))", s_pos, s_pos, s_pos);
						break;
					case ptCUBE:
						asprintf(&c, "clrd(sC(%d),d2Cube(pos,sP(%d),sS(%d)))", s_pos, s_pos, s_pos);
						break;
					case ptTORUS:
					case ptCTORUS:
					case ptCYL:
					case ptCCONE:
						die("primitive type %d not implemented", pos->p.type);
						break;
				}
				s_pos++;
				strcat(sdf, c);
				xfree(c);
				break;
			case stGROUP:
				switch (pos->g.type) {
					case gtUNION:       asprintf(&c, "u(");           break;
					case gtDIFF:        asprintf(&c, "d(");           break;
					case gtINTERS:      asprintf(&c, "i(");           break;
					case gtBLEND:       asprintf(&c, "b(gK(%d),", g_pos); break;
					case gtAVERAGE:     asprintf(&c, "a(gK(%d),", g_pos); break;
							    //case gtAPPROXIMATE: asprintf(&c, "x(");           break;
					case gtAPPROXIMATE: die("group type approximate not implemented");
				}
				g_pos++;
				strcat(sdf, c);
				xfree(c);

				sdf_gen(pos->g.a);
				strcat(sdf, ",");
				sdf_gen(pos->g.b);
				strcat(sdf, ")");
				break;
			case stWRAPPER:
				die("wrappers are not implemented");
				break;
		}
	}

	sdf_gen(s->root);

	return xrealloc(sdf, strlen(sdf) + 1);
}

void scene_compile(Scene* s) {
	char* shader_code = xmalloc_zero(template_glsl_len + 1024);
	char* inserts[7] = {0};

	asprintf(&(inserts[0]), "%.9f", s->eps       );
	asprintf(&(inserts[1]), "%.9f", s->max_dist  );
	asprintf(&(inserts[2]), "%d"  , s->rm_iters  );
	asprintf(&(inserts[3]), "%d"  , s->main_iters);
	asprintf(&(inserts[4]), "%d"  , s->primt_count);
	asprintf(&(inserts[5]), "%d"  , s->group_count);
	inserts[6] = scene_create_sdf(s);

	int i = 0;
	for (unsigned int j = 0; j < template_glsl_len; j++) {
		if (template_glsl[j] == '@') {
			strcat(shader_code, inserts[i]);
			i++;
		} else {
			char tmp[2] = {template_glsl[j], '\0'};
			strcat(shader_code, tmp);
		}
	}

	for (int k = 0; k < 7; k++) xfree(inserts[k]);

	UnloadShader(s->shader);
	s->shader = LoadShaderFromMemory(0, shader_code);

#ifdef R8_DEBUG
	FILE *fp = fopen("shader.glsl", "w+");
	fputs(shader_code, fp);
	fclose(fp);
#endif

	xfree(shader_code);

	s->res_loc     = GetShaderLocation(s->shader, "resolution");
	s->ro_loc      = GetShaderLocation(s->shader, "viewEye");
	s->ta_loc      = GetShaderLocation(s->shader, "viewCenter");
	s->prims_loc   = GetShaderLocation(s->shader, "prims");
	s->groups_loc  = GetShaderLocation(s->shader, "groups");
	s->portals_loc = GetShaderLocation(s->shader, "portals");
	//s->time_loc = GetShaderLocation(s->shader, "time");

	float res[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
	SetShaderValue(s->shader, s->res_loc, res, SHADER_UNIFORM_VEC2);
}

void scene_print(Scene* s) {
	void rec(Shape *pos, int depth) {
		for (int i = 0; i < depth * 4; i++) printf(" ");
		if (pos == NULL) {
			printf("NULL\n");
			return;
		}
		switch (pos->type) {
			case stPRIMITIVE:
				printf("primt\n");
				break;
			case stGROUP:
				printf("group\n");
				rec(pos->g.a, depth + 1);
				rec(pos->g.b, depth + 1);
				break;
			case stWRAPPER:
				printf("wrapp\n");
				rec(pos->w.shape, depth + 1);
				break;
		}
	}

	rec(s->root, 0);
}

void scene_on_tree_update(Scene *s) {
	// === Count shapes ===
	unsigned int wrapper_count = 0, group_count = 0, prim_count = 0;
	void count_shapes(Shape *pos) {
		switch (pos->type) {
			case stPRIMITIVE:
				prim_count++;
				break;
			case stGROUP:
				group_count++;
				count_shapes(pos->g.a);
				count_shapes(pos->g.b);
				break;
			case stWRAPPER:
				wrapper_count++;
				count_shapes(pos->w.shape);
				break;
			default:
				die("invalid shape type %d", pos->type);
		}
	}
	count_shapes(s->root);
	s->primt_count = prim_count;
	s->group_count = group_count;

	// === Update flat_{prims,groups} ===
	Shape **nflat_prims  = xrealloc(s->flat_prims , sizeof(Shape*) * s->primt_count);
	Shape **nflat_groups = xrealloc(s->flat_groups, sizeof(Shape*) * s->group_count);
	int pi = 0, gi = 0;
	void iter_shapes(Shape *pos) {
		switch (pos->type) {
			case stPRIMITIVE:
				nflat_prims[pi++] = pos;
				break;
			case stGROUP:
				nflat_groups[gi++] = pos;
				iter_shapes(pos->g.a);
				iter_shapes(pos->g.b);
				break;
			case stWRAPPER:
				iter_shapes(pos->w.shape);
				break;
			default:
				die("invalid shape type %d", pos->type);
		}
	}
	iter_shapes(s->root);
	s->flat_prims  = nflat_prims;
	s->flat_groups = nflat_groups;

	s->tree_changed  = true;
	s->primt_changed = true;
	s->group_changed = true;
}

void scene_tick(Scene* s) {
	// === Update camera ===
	UpdateCamera(&(s->cam));

	float ro[3] = { s->cam.position.x, s->cam.position.y, s->cam.position.z };
	float ta[3] = { s->cam.target.x,   s->cam.target.y,   s->cam.target.z };

	SetShaderValue(s->shader, s->ro_loc, ro, SHADER_UNIFORM_VEC3);
	SetShaderValue(s->shader, s->ta_loc, ta, SHADER_UNIFORM_VEC3);

	// === Update window size ===
	if (IsWindowResized()) {
		float res[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
		SetShaderValue(s->shader, s->res_loc, res, SHADER_UNIFORM_VEC2);
	}

	// === Update primitives ===
	if (s->primt_changed) {
		float primts[s->primt_count * PRIMT_SIZE];
		for (unsigned int i = 0; i < s->primt_count; i++) {
			for (unsigned int j = 0; j < PRIMT_SIZE; j++) {
				primts[i * PRIMT_SIZE + j] = s->flat_prims[i]->p.d[j];
				//printf("%.2f ", s->flat_prims[i]->p.d[j]);
			}
			//printf("\n");
		}
		SetShaderValueV(s->shader, s->prims_loc, primts, SHADER_UNIFORM_FLOAT, (int)(s->primt_count * PRIMT_SIZE));

		s->primt_changed = false;
	}

	// === Update groups ===
	if (s->group_changed) {
		float groups[s->group_count];
		for (unsigned int i = 0; i < s->group_count; i++) {
			//printf("%p ", s->flat_groups[i]);
			groups[i] = s->flat_groups[i]->g.k;
		}
		//printf("\n");

		SetShaderValueV(s->shader, s->groups_loc, groups, SHADER_UNIFORM_FLOAT, (int)(s->group_count));

		s->group_changed = false;
	}

	// === Update portals ===
	if (s->portal_changed) {
		// a valid portals is a portal that links to another portal that links back to this portal
		Portal **valid_portals = xmalloc_zero(sizeof(Portal) * MAX_PORTALS_PER_SCENE);

		int j = 0;
		for (int i = 0; i < MAX_PORTALS_PER_SCENE; i++) { // iterate over all portals
			Portal *p = s->portals[i];
			if (p == NULL) continue;
			if (p->link->link == p) { // check that it is a valid portal
				bool seen = false; // check that we did not add this pair yet
				for (int k = 0; k < MAX_PORTALS_PER_SCENE; k++) {
					if (valid_portals[k] == p) {
						seen = true;
						break;
					}
				}
				if (seen) continue;
				// add this pair
				valid_portals[j] = p;
				valid_portals[j + 1] = p->link;
				j += 2;
			}
		}
		float *portals = portal_write_bulk(valid_portals, j);
		xfree(valid_portals);

		/*for (unsigned int i = 0; i < j; i++) {
			for (unsigned int k = 0; k < PORTAL_SIZE; k++) {
				printf("%.2f ", portals[i * PORTAL_SIZE + k]);
			}
			printf("\n");
		}*/

		SetShaderValueV(s->shader, s->portals_loc, portals, SHADER_UNIFORM_VEC3, j * 4);
		xfree(portals);

		s->portal_changed = false;
	}
}

void scene_destroy(Scene* s) {
	void free_shapes(Shape *pos) {
		switch (pos->type) {
			case stPRIMITIVE:
				break;
			case stGROUP:
				free_shapes(pos->g.a);
				free_shapes(pos->g.b);
				break;
			case stWRAPPER:
				free_shapes(pos->w.shape);
				break;
			default:
				die("invalid shape type %d", pos->type);
		}
		xfree(pos);
	}
	free_shapes(s->root); // TODO: maybe use the cached arrays?
	UnloadShader(s->shader);
	xfree(s->flat_prims);
	xfree(s->flat_groups);
	for (int i = 0; i < MAX_PORTALS_PER_SCENE; i++)
		if (s->portals[i])
			xfree(s->portals[i]);
	xfree(s->portals);
	xfree(s);
}

