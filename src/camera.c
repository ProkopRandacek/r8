#include <stdio.h>
#include <stdlib.h>

#include "camera.h"


float lastAngle, lastH, lastW;

extern int w, h; // these are from opengl.c. I need to change these for the screenshots to be the right size

Camera cmr(vec3 pos, vec3 dir, float angle, float he, float wi) {
	lastAngle = angle;
	lastH = he;
	lastW = wi;

	Camera cam;
	cam.pos = pos;

	vec3 sc = vAdd(pos, dir); // Screen center
	// SIDE VIEW
	// Camera        ..--``t <- top points
	// |       ..--``      |
	// V ..--``            |
	// C- - - dir vec - ->SC <- Screen center point
	//   ``--..            |
	//         ``--..      |
	//               ``--..b <- bottom points

	//float y = 1.0f / tan(angle); calculate angle TODO

	vec3 left = vMultf(vNorm(v3(-dir.z, angle, dir.x)), wi);
	// CAMERA VIEW
	// tl---------tr // top left, top right
	// |           | // SC = Screen center point
	// |           |
	// <----SC     |
	// |           |
	// |           |
	// bl---------br // bottom left, bottom right

	vec3 up = vMultf(vNorm(vCross(left, dir)), he);
	// tl----^----tr picture in camera space
	// |     |     | // SC = Screen center point
	// |     |     |
	// |    SC     |
	// |           |
	// |           |
	// bl---------br

	vec3 right = vMultf(left, -1.0f);
	vec3 down = vMultf(up, -1.0f);

	cam.left = vNorm(left);
	cam.forward = vNorm(dir);

	cam.tl = vAdd(vAdd(up, left), sc);
	cam.tr = vAdd(vAdd(up, right), sc);
	cam.bl = vAdd(vAdd(down, left), sc);
	cam.br = vAdd(vAdd(down, right), sc);
	return cam;
}

void updateCamPos(Camera* cam, vec3 offset) {
	vec3 forward = v3(cam->forward.x, 0, cam->forward.z);
	vec3 left = v3(cam->left.x, 0, cam->left.z);
	cam->pos = vAdd(cam->pos, vMultf(forward, offset.z));
	cam->pos = vAdd(cam->pos, vMultf(left, -offset.x));
	cam->pos.y += offset.y;
}

void setWH(float iw, float ih) {
	lastH = ih;
	lastW = iw;
	w = (int) (iw * 1000.0f);
	h = (int) (ih * 1000.0f);
}

Camera updateCamDir(vec3 pos, vec3 dir) { return cmr(pos, dir, lastAngle, lastH, lastW); }
