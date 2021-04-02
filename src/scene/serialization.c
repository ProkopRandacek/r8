#include <stdint.h>
#include <stdio.h>

#include "camera.h"
#include "serialization.h"
#include "../debug.h"
#include "../opengl.h"
#include "../settings.h"

extern GL* gl;

void sphere2floats(float* f, Sphere* s) {
	f[6 ] = 0.0f;      f[7 ] = 0.0f;     f[8 ] = 0.0f;
	f[9 ] = s->radius; f[10] = 0.0f;     f[11] = 0.0f;
	f[12] = 0.0f;      f[13] = 0.0f;     f[14] = 0.0f;
}

void cube2floats(float* f, Cube* s) {
	f[6 ] = s->scale.x;   f[7 ] = s->scale.y; f[8 ] = s->scale.z;
	f[9 ] = s->roundEdge; f[10] = 0.0f;       f[11] = 0.0f;
	f[12] = 0.0f;         f[13] = 0.0f;       f[14] = 0.0f;
}

void torus2floats(float* f, Torus* s) {
	f[6 ] = 0.0f;      f[7 ] = 0.0f;       f[8 ] = 0.0f;
	f[9 ] = s->radius; f[10] = s->fatness; f[11] = 0.0f;
	f[12] = 0.0f;      f[13] = 0.0f;       f[14] = 0.0f;
}

void caps2floats(float* f, Capsule* s) {
	f[6 ] = s->end.x;   f[7 ] = s->end.y;   f[8 ] = s->end.z;
	f[9 ] = s->radius;  f[10] = 0.0f;       f[11] = 0.0f;
	f[12] = 0.0f;       f[13] = 0.0f;       f[14] = 0.0f;
}

void cyl2floats(float* f, Cylinder* s) {
	f[6 ] = s->end.x;   f[7 ] = s->end.y;   f[8 ] = s->end.z;
	f[9 ] = s->radius;  f[10] = 0.0f;       f[11] = 0.0f;
	f[12] = 0.0f;       f[13] = 0.0f;       f[14] = 0.0f;
}

void ccone2floats(float* f, CCone*s) {
	f[6 ] = s->end.x;   f[7 ] = s->end.y;   f[8 ] = s->end.z;
	f[9 ] = s->startR;  f[10] = s->endR;    f[11] = 0.0f;
	f[12] = 0.0f;       f[13] = 0.0f;       f[14] = 0.0f;
}


void groups2floats(int* f, unsigned int num, ShapeGroup* groups[num]) {
	for (unsigned int i = 0; i < num; i++) {
		unsigned int p = i * GROUP_SIZE;
		f[p + 0] = groups[i]->a;
		f[p + 1] = groups[i]->b;
		f[p + 2] = groups[i]->op;
		f[p + 3] = (int)(groups[i]->k * 1000.0);

		for (int j = 0; j < GROUP_SIZE; j++) {
			//printf("%d, ", f[p + j]);
		}
		//printf("\n");
	}
	//printf("\n");
}

void shapes2floats(float *f, unsigned int num, Shape* shp[num]) {
	for (unsigned int i = 0; i < num; i++) {
		// for all shapes same:
		unsigned int p = i * SHAPE_SIZE;
		f[p + 0 ] = shp[i]->pos.x; f[p + 1] = shp[i]->pos.y; f[p + 2] = shp[i]->pos.z; // position
		f[p + 3 ] = shp[i]->clr.x; f[p + 4] = shp[i]->clr.y; f[p + 5] = shp[i]->clr.z; // color
		f[p + 15] = shp[i]->rv; // refletiveness

		// shape specific:
		if      (shp[i]->type == CUBE)     {   cube2floats(&f[p],     (Cube*) shp[i]->shape); }
		else if (shp[i]->type == SPHERE)   { sphere2floats(&f[p],   (Sphere*) shp[i]->shape); }
		else if (shp[i]->type == CYLINDER) {    cyl2floats(&f[p], (Cylinder*) shp[i]->shape); }
		else if (shp[i]->type == TORUS)    {  torus2floats(&f[p],    (Torus*) shp[i]->shape); }
		else if (shp[i]->type == CCONE)    {  ccone2floats(&f[p],    (CCone*) shp[i]->shape); }
		else {
			char errMsg[62];
			sprintf(errMsg, "Unknown shape %d on index %d (shapes2floats)", shp[i]->type, i);
			eprint(errMsg);
		}
		for (int j = 0; j < SHAPE_SIZE; j++) {
			//printf("%.2f, ", f[p + j]);
		}
		//printf("\n");
	}
	//printf("\n");
}

void shapes2types(int *f, unsigned int num, Shape* shp[num]) {
	for (unsigned int i = 0; i < num; i++) {
		f[i] = (int)shp[i]->type;
		//printf("%d, ", f[i]);
	}
	//printf("\n");
}

void cam2floats(Camera* cam, float* f) {
	f[0]  = cam->pos.x; f[1]  = cam->pos.y; f[2] = cam->pos.z;
	f[3]  = cam->tr.x;  f[4]  = cam->tr.y;  f[5] = cam->tr.z;
	f[6]  = cam->tl.x;  f[7]  = cam->tl.y;  f[8] = cam->tl.z;
	f[9]  = cam->br.x;  f[10] = cam->br.y; f[11] = cam->br.z;
	f[12] = cam->bl.x;  f[13] = cam->bl.y; f[14] = cam->bl.z;
}
