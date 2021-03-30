// vim: filetype=c

#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct Cameras {
	vec3 pos;
	vec3 tl; // The direction of most top left ray
	vec3 tr; // top right
	vec3 bl; // ...
	vec3 br; // ...

	vec3 left;
	vec3 forward;
} Camera;

Camera* cmr(vec3 pos, vec3 dir, float angle, float h, float w);
void updateCamPos(Camera* cam, vec3 offset);
Camera* updateCamDir(vec3 pos, vec3 dir);
void setWH(float w, float h);

#endif
