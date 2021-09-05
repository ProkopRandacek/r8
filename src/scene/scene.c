#define _GNU_SOURCE

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "alloc.h"
#include "log.h"
#include "shapes.h"
#include "stack.h"
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
	s->cam.target   = (Vector3){ 0.0f, 0.0f, 2.0f };
	s->cam.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
	s->cam.fovy     = 65.0f;

	SetCameraMode(s->cam, CAMERA_FREE);

	s->tree_changed   = true;
	s->primt_changed  = true;
	s->group_changed  = true;
	s->portal_changed = true;

	return s;
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
	s->shader = LoadShaderFromMemory(0, shader_code); // This leaks FIXME

#if 1
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

// Internal only, use scene_print(Scene* s)
void scene_print_rec(Shape *pos, int depth) {
	for (int i = 0; i < depth; i++) printf(">>>>");
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
			scene_print_rec(pos->g.a, depth + 1);
			scene_print_rec(pos->g.b, depth + 1);
			break;
		case stWRAPPER:
			printf("wrapp\n");
			scene_print_rec(pos->w.shape, depth + 1);
			break;
	}
}

void scene_print(Scene* s) {
	scene_print_rec(s->root, 0);
}

void scene_on_tree_update(Scene *s) {
	{
		// === Count shapes ===
		unsigned int wrapper_count = 0, group_count = 0, prim_count = 0;
		Stack *stack = stack_new();
		stack_push(stack, s->root);
		while (stack->top > 0) {
			Shape *pos = stack_pop(stack);
			if (pos == NULL || !pos->visible) continue;
			switch (pos->type) {
				case stPRIMITIVE:
					prim_count++;
					break;
				case stGROUP:
					group_count++;
					stack_push(stack, pos->g.a);
					stack_push(stack, pos->g.b);
					break;
				case stWRAPPER:
					wrapper_count++;
					stack_push(stack, pos->w.shape);
					break;
				default:
					die("invalid shape type %d", pos->type);
			}
		}
		stack_destroy(stack);
		s->primt_count = prim_count;
		s->group_count = group_count;
	}

	{
		// === Update flat_{prims,groups} ===
		Shape **nflat_prims  = xrealloc(s->flat_prims , sizeof(Shape*) * s->primt_count);
		Shape **nflat_groups = xrealloc(s->flat_groups, sizeof(Shape*) * s->group_count);
		int pi = 0, gi = 0;
		Stack *stack = stack_new();
		stack_push(stack, s->root);
		while (stack->top > 0) {
			Shape *pos = stack_pop(stack);
			if (pos == NULL || !pos->visible) continue;
			switch (pos->type) {
				case stPRIMITIVE:
					nflat_prims[pi++] = pos;
					break;
				case stGROUP:
					nflat_groups[gi++] = pos;
					stack_push(stack, pos->g.b);
					stack_push(stack, pos->g.a);
					break;
				case stWRAPPER:
					stack_push(stack, pos->w.shape);
					break;
				default:
					die("invalid shape type %d", pos->type);
			}
		}
		stack_destroy(stack);
		s->flat_prims  = nflat_prims;
		s->flat_groups = nflat_groups;
	}

	s->tree_changed  = true;
	s->primt_changed = true;
	s->group_changed = true;
	scene_compile(s);
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

#if 0
		for (unsigned int i = 0; i < j; i++) {
			for (unsigned int k = 0; k < PORTAL_SIZE; k++) {
				printf("%.2f ", portals[i * PORTAL_SIZE + k]);
			}
			printf("\n");
		}
#endif

		SetShaderValueV(s->shader, s->portals_loc, portals, SHADER_UNIFORM_VEC3, j * 4);
		xfree(portals);

		s->portal_changed = false;
	}
}

void scene_delete_shape(Scene* s, Shape* d) {
	if (d == s->root) {
		msg("Can't delete root shape");
		return;
	}

	Stack *stack = stack_new();
	stack_push(stack, s->root);
	while (stack->top > 0) {
		Shape *pos = stack_pop(stack);
		if (pos == NULL) continue;
		switch (pos->type) {
			case stPRIMITIVE:
				break;
			case stGROUP:
				if (pos->g.b == d) {
					pos->g.b = NULL;
					goto end;
				}
				if (pos->g.a == d) {
					pos->g.a = NULL;
					goto end;
				}
				stack_push(stack, pos->g.b);
				stack_push(stack, pos->g.a);
				break;
			case stWRAPPER:
				if (pos->w.shape == d) {
					pos->w.shape = NULL;
					goto end;
				}
				stack_push(stack, pos->w.shape);
				break;
			default:
				die("invalid shape type %d", pos->type);
		}
	}
end:
	stack_destroy(stack);

	stack = stack_new();
	stack_push(stack, d);
	while (stack->top > 0) {
		Shape *pos = stack_pop(stack);
		if (pos == NULL) continue;
		switch (pos->type) {
			case stPRIMITIVE:
				break;
			case stGROUP:
				stack_push(stack, pos->g.a);
				stack_push(stack, pos->g.b);
				break;
			case stWRAPPER:
				stack_push(stack, pos->w.shape);
				break;
			default:
				die("invalid shape type %d", pos->type);
		}
		xfree(pos);
	}
	stack_destroy(stack);
	scene_on_tree_update(s);
}

void scene_destroy(Scene* s) {
	Stack *stack = stack_new();
	stack_push(stack, s->root);
	while (stack->top > 0) {
		Shape *pos = stack_pop(stack);
		if (pos == NULL) continue;
		switch (pos->type) {
			case stPRIMITIVE:
				break;
			case stGROUP:
				stack_push(stack, pos->g.a);
				stack_push(stack, pos->g.b);
				break;
			case stWRAPPER:
				stack_push(stack, pos->w.shape);
				break;
			default:
				die("invalid shape type %d", pos->type);
		}
		xfree(pos);
	}
	stack_destroy(stack);
	UnloadShader(s->shader);
	xfree(s->flat_prims);
	xfree(s->flat_groups);
	for (int i = 0; i < MAX_PORTALS_PER_SCENE; i++)
		if (s->portals[i])
			xfree(s->portals[i]);
	xfree(s->portals);
	xfree(s);
}

