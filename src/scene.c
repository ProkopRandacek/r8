#define _GNU_SOURCE
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "template.glsl.h"

Scene *scene_new() {
	Scene *s = xmalloc_zero(sizeof(Scene));

	s->eps = 0.01f;
	s->max_dist = 64.0f;
	s->rm_iters = 256;
	s->main_iters = 8;

	s->root = NULL;

	// default camera settings
	s->cam.position = (Vector3){ 0.0f, 2.0f, 0.0f };
	s->cam.target   = (Vector3){ 0.0f, 2.0f, 1.0f };
	s->cam.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
	s->cam.fovy     = 65.0;

	SetCameraMode(s->cam, CAMERA_FIRST_PERSON);

	scene_compile(s);

	return s;
}

char* build_primitive(Primitive p) {
	char *c;
	switch (p.type) {
		case ptSPHERE:
			asprintf(&c, "clrd(vec4(),d2Shape(pos,vec3(-2, 1, 0),vec3(1.5))");
			break;
		case ptCUBE:
			asprintf(&c, "clrd(vec4(),d2Cube(pos,vec3(2, 1, 0),1.5)");
			break;
		case ptTORUS:
		case ptCTORUS:
		case ptCYL:
		case ptCCONE:
			die("primitive type %d not implemented", p.type);
			break;
	}
	return c;
}

char* build_group(Group g) {
	char *c;
	switch (g.type) {
		case gtUNION:       asprintf(&c, "unin(");              break;
		case gtDIFF:        asprintf(&c, "diff(");              break;
		case gtINTERS:      asprintf(&c, "inters(");            break;
		case gtBLEND:       asprintf(&c, "blend(%.9f,", g.k);   break;
		case gtAVERAGE:     asprintf(&c, "average(%.9f,", g.k); break;
		case gtAPPROXIMATE: asprintf(&c, "approximate(");       break;
	}
	return c;
}

void sdf_gen(char* sdf, Shape *pos) {
	switch (pos->type) {
		case stPRIMITIVE: ;
			char* p = build_primitive(pos->p);
			strcat(sdf, p);
			xfree(p);
			break;
		case stGROUP: ;
			char* g = build_group(pos->g);
			strcat(sdf, g);
			xfree(g);

			sdf_gen(sdf, pos->g.a);
			strcat(sdf, ",");
			sdf_gen(sdf, pos->g.b);
			strcat(sdf, ")");
			break;
		case stWRAPPER:
			die("wrappers are not implemented");
			break;
	}
}

char* scene_create_sdf(Scene *s) {
	char* sdf = xmalloc(8192);
	sdf_gen(sdf, s->root);
	return sdf;
}

void scene_compile(Scene* s) {
	char* shader_code = xmalloc_zero(template_glsl_len + 1024);
	char* inserts[4] = {0};

	asprintf(&(inserts[0]), "%.9f", s->eps       );
	asprintf(&(inserts[1]), "%.9f", s->max_dist  );
	asprintf(&(inserts[2]), "%d"  , s->rm_iters  );
	asprintf(&(inserts[3]), "%d"  , s->main_iters);

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

	for (int k = 0; k < 4; k++) xfree(inserts[k]);

	UnloadShader(s->shader);
	s->shader = LoadShaderFromMemory(0, shader_code);
	xfree(shader_code);

	s->resLoc = GetShaderLocation(s->shader, "resolution");
	s->roLoc  = GetShaderLocation(s->shader, "viewEye");
	s->taLoc  = GetShaderLocation(s->shader, "viewCenter");

	float res[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
	SetShaderValue(s->shader, s->resLoc, res, SHADER_UNIFORM_VEC2);
}

void scene_print(Shape *pos, int depth) {
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
			scene_print(pos->g.a, depth + 1);
			scene_print(pos->g.b, depth + 1);
			break;
		case stWRAPPER:
			printf("wrapp\n");
			scene_print(pos->w.shape, depth + 1);
			break;
	}
}

void scene_count_shapes(Shape *pos, int *shape_count, int *group_count) {
	switch (pos->type) {
		case stPRIMITIVE:
			(*shape_count)++;
			break;
		case stGROUP:
			(*group_count)++;
			scene_count_shapes(pos->g.a, shape_count, group_count);
			scene_count_shapes(pos->g.b, shape_count, group_count);
			break;
		case stWRAPPER:
			scene_count_shapes(pos->w.shape, shape_count, group_count);
			break;
		default:
			die("invalid shape type %d", pos->type);
	}
}

void scene_write_shapes(Shape *pos, float *shapes, float *groups, int *shapei, int *groupi) {
	switch (pos->type) {
		case stPRIMITIVE:
			for (int i = 0; i < SHAPE_SIZE; i++)
				shapes[*shapei + i] = pos->p.d[i];
			*shapei += SHAPE_SIZE;
			break;
		case stGROUP:
			groups[*groupi] = pos->g.k;
			*groupi += 1;
			scene_write_shapes(pos->g.a, shapes, groups, shapei, groupi);
			scene_write_shapes(pos->g.b, shapes, groups, shapei, groupi);
			break;
		case stWRAPPER:
			scene_write_shapes(pos->w.shape, shapes, groups, shapei, groupi);
			break;
	}
}

void scene_update(Scene* s) {
	UpdateCamera(&(s->cam));

	float ro[3] = { s->cam.position.x, s->cam.position.y, s->cam.position.z };
	float ta[3] = { s->cam.target.x,   s->cam.target.y,   s->cam.target.z };

	SetShaderValue(s->shader, s->roLoc, ro, SHADER_UNIFORM_VEC3);
	SetShaderValue(s->shader, s->taLoc, ta, SHADER_UNIFORM_VEC3);

	if (IsWindowResized()) {
		float res[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
		SetShaderValue(s->shader, s->resLoc, res, SHADER_UNIFORM_VEC2);
	}

	int shape_count = 0, group_count = 0;
	// first walk the tree to count shapes and groups
	scene_count_shapes(s->root, &shape_count, &group_count);

	float shapes[shape_count * SHAPE_SIZE];
	float groups[group_count * 1         ];
	for (int i = 0; i < shape_count * SHAPE_SIZE; i++) shapes[i] = 0.0f;
	for (int i = 0; i < group_count * 1         ; i++) groups[i] = 0.0f;

	int shapei = 0, groupi = 0;
	// second walk the tree to write shapes and groups into float arrays
	scene_write_shapes(s->root, shapes, groups, &shapei, &groupi);
	printf("%d shapes %d groups\n", shape_count, group_count);

	for (int i = 0; i < shape_count; i++) {
		for (int j = 0; j < SHAPE_SIZE; j++) {
			printf("%.1f \t", shapes[i * SHAPE_SIZE + j]);
		}
		printf("\n");
	}

	scene_print(s->root, 0);
}

void scene_destroy(Scene* s) {
	UnloadShader(s->shader);
	xfree(s);
}

