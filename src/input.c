#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "camera.h"
#include "opengl.h"
#include "time.h"
#include "settings.h"


extern GL* gl;
extern Camera* cam;
extern int w, h;
extern float deltaTime;

char wDown = 0;
char aDown = 0;
char sDown = 0;
char dDown = 0;
char shiftDown = 0;
char spaceDown = 0;

void updateInput() {
	double xpos, ypos;

	// wasd
	vec3 moveDir = v3(0.0f, 0.0f, 0.0f);
	if (wDown) moveDir.z = 1.0f;
	if (sDown) moveDir.z = -1.0f;
	if (aDown) moveDir.x = 1.0f;
	if (dDown) moveDir.x = -1.0f;
	if (spaceDown) moveDir.y = 1.0f;
	if (shiftDown) moveDir.y = -1.0f;
	if (vMag(moveDir) > 0.0f) {
		moveDir = vMultf(vNorm(moveDir), MOVE_SPEED * deltaTime);
		updateCamPos(cam, moveDir);
	}

	// mouse
	glfwGetCursorPos(gl->window, &xpos, &ypos);
	float x = ((2.0f * (float)xpos) / (float)w - 1.0f) * -MOUSE_SENS;
	float y = (1.0f - (2.0f *(float) ypos) /(float) h) * MOUSE_SENS;

	vec3 dir = vNorm(v3(
		cosf(y) * sinf(x),
		sinf(y),
		cosf(y) * cosf(x)
	));
	cam = updateCamDir(cam->pos, dir);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
// ignore unused parameter warning for callback function
void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (window != gl->window) return;
	if (action == 2) return;

	//printf("%d, %d, %d, %d\n", key, scancode, action, mods);

#ifdef _WIN32
	const int scQ = 16;
	const int scW = 17;
	const int scA = 30;
	const int scS = 31;
	const int scD = 32;
	//const int scE = 18;
	const int scShift = 42;
	const int scSpace = 57;
	const int scP = 25;
#else
	const int scQ = 24;
	const int scW = 25;
	const int scA = 38;
	const int scS = 39;
	const int scD = 40;
	//const int scE = 26;
	const int scShift = 50;
	const int scSpace = 65;
	const int scP = 33;
#endif

	if (scancode == scQ && action == 1) glfwSetWindowShouldClose(gl->window, GL_TRUE);

	else if (scancode == scW && action == 1) wDown = 1;
	else if (scancode == scA && action == 1) aDown = 1;
	else if (scancode == scS && action == 1) sDown = 1;
	else if (scancode == scD && action == 1) dDown = 1;
	else if (scancode == scShift && action == 1) shiftDown = 1;
	else if (scancode == scSpace && action == 1) spaceDown = 1;

	else if (scancode == scW && action == 0) wDown = 0;
	else if (scancode == scA && action == 0) aDown = 0;
	else if (scancode == scS && action == 0) sDown = 0;
	else if (scancode == scD && action == 0) dDown = 0;
	else if (scancode == scShift && action == 0) shiftDown = 0;
	else if (scancode == scSpace && action == 0) spaceDown = 0;

	else if (scancode == scP && action == 1) screenshot();
}

