#include "camera.h"

#include "vector.h"

extern int w, h; // these are from opengl.c. the window width and height

void cmr(Camera* cam, vec3 pos, vec3 dir, float angle) {
	cam->pos = pos;

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

	vec3 left = vMultf(vNorm(v3(-dir.z, angle, dir.x)), ((float)w / 1000.0f));
	// CAMERA VIEW
	// tl---------tr // top left, top right
	// |           | // SC = Screen center point
	// |           |
	// <----SC     |
	// |           |
	// |           |
	// bl---------br // bottom left, bottom right

	vec3 up = vMultf(vNorm(vCross(left, dir)), ((float)h / 1000.0f));
	// tl----^----tr picture in camera space
	// |     |     | // SC = Screen center point
	// |     |     |
	// |    SC     |
	// |           |
	// |           |
	// bl---------br

	vec3 right = vMultf(left, -1.0f);
	vec3 down  = vMultf(up, -1.0f);

	cam->left    = vNorm(left);
	cam->forward = vNorm(dir);

	cam->tl = vAdd(vAdd(up,    left), sc);
	cam->tr = vAdd(vAdd(up,   right), sc);
	cam->bl = vAdd(vAdd(down,  left), sc);
	cam->br = vAdd(vAdd(down, right), sc);
}

