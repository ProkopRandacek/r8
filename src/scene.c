#define _GNU_SOURCE
#include <ucw/lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "shaders/template.glsl.h"

Scene *scene_new() {
	Scene *s = malloc(sizeof(Scene));

	s->eps = 0.01;
	s->max_dist = 64.0;
	s->rm_iters = 256;
	s->main_iters = 8;

	s->root_shape = NULL;

	return s;
}

void sdf_gen(char* sdf, Shape *pos) {
	switch (pos->type) {
		case stPRIMITIVE:
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
}

char* scene_compile(Scene* s) {
	char* shader = calloc(sizeof(char), template_glsl_len + 1024);
	char* inserts[4] = {0};

	asprintf(&(inserts[0]), "%.9f", s->eps       );
	asprintf(&(inserts[1]), "%.9f", s->max_dist  );
	asprintf(&(inserts[2]), "%d"  , s->rm_iters  );
	asprintf(&(inserts[3]), "%d"  , s->main_iters);

	int i = 0;
	for (int j = 0; j < template_glsl_len; j++) {
		if (template_glsl[j] == '@') {
			strcat(shader, inserts[i]);
			i++;
		} else {
			char tmp[2] = {template_glsl[j], '\0'};
			strcat(shader, tmp);
		}
	}

	for (int k = 0; k < 4; k++) free(inserts[k]);

	return realloc(shader, strlen(shader));
}

