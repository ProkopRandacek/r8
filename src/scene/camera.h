// vim: filetype=c
#ifndef CAMERA_H
#define CAMERA_H

#include "../vector.h"

typedef struct Cameras {
	vec3 pos;
	vec3 tl; // The direction of most top left ray
	vec3 tr; // top right
	vec3 bl; // ...
	vec3 br; // ...

	vec3 left; // these are for C, not actually send to the GPU
	vec3 forward;
} Camera;

void cmr(Camera* cam, vec3 pos, vec3 dir, float angle);
//void updateCamPos(Camera* cam, vec3 offset);
//void updateCamDir(Camera* cam, vec3 pos, vec3 dir);
//void setWH(Camera* cam, float w, float h);

#endif
