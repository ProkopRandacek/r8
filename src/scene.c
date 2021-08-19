#define _GNU_SOURCE
#include <ucw/lib.h>

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "template.glsl.h"

Scene *scene_new() {
	Scene *s = calloc(1, sizeof(Scene));

	s->eps = 0.01;
	s->max_dist = 64.0;
	s->rm_iters = 256;
	s->main_iters = 8;

	s->root_shape = NULL;

	// default camera settings
	s->cam.position = (Vector3){ 0.0f, 2.0f, 0.0f };
	s->cam.target   = (Vector3){ 0.0f, 2.0f, 1.0f };
	s->cam.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
	s->cam.fovy     = 65.0;

	SetCameraMode(s->cam, CAMERA_FIRST_PERSON);

	scene_compile(s);

	return s;
}

/*
char* build_primitive(Primitive p) {
}

void sdf_gen(char* sdf, Shape *pos) {
	switch (pos->type) {
		case stPRIMITIVE:
			char* p = build_primitive(pos->primitive);
			strcat(sdf, p);
			free(p);
			break;
		case stGROUP:
			break;
		case stWRAPPER:
			die("wrappers not implemented");
			break;
	}
}

char* scene_create_sdf(Scene *s) {
	char* sdf = malloc(8192);

	sdf_gen(sdf, s->root_shape);

	return sdf;
}*/

void scene_compile(Scene* s) {
	char* shader_code = calloc(sizeof(char), template_glsl_len + 1024);
	char* inserts[4] = {0};

	asprintf(&(inserts[0]), "%.9f", s->eps       );
	asprintf(&(inserts[1]), "%.9f", s->max_dist  );
	asprintf(&(inserts[2]), "%d"  , s->rm_iters  );
	asprintf(&(inserts[3]), "%d"  , s->main_iters);

	int i = 0;
	for (int j = 0; j < template_glsl_len; j++) {
		if (template_glsl[j] == '@') {
			strcat(shader_code, inserts[i]);
			i++;
		} else {
			char tmp[2] = {template_glsl[j], '\0'};
			strcat(shader_code, tmp);
		}
	}

	for (int k = 0; k < 4; k++) free(inserts[k]);

	UnloadShader(s->shader);
	s->shader = LoadShaderFromMemory(0, shader_code);
	free(shader_code);

	s->resLoc = GetShaderLocation(s->shader, "resolution");
	s->roLoc  = GetShaderLocation(s->shader, "viewEye");
	s->taLoc  = GetShaderLocation(s->shader, "viewCenter");
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
}

void scene_destroy(Scene* s) {
	UnloadShader(s->shader);
	free(s);
}

