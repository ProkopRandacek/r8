#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#include "debug.h"
#include "main.h"
#include "settings.h"
#include "umka/umka.h"

extern GL* gl;
extern void* umka;
float deltaTime = 0.0f;
extern int umkaStartFunc, umkaUpdateFunc;

int main() {
	printf("\n\n====================================\n\n\nbuild name: %s\nI think I'm runnig on ", BUILD_NAME);
#ifdef _WIN32
	printf("windows\n");
#else
	printf("Linux\n");
#endif

	Scene s;
	s.shapeMask = CUBE_MASK | SPHERE_MASK | CYLINDER_MASK | TORUS_MASK | CCONE_MASK;
	s.maxShapeNum = MAX_SHAPE_NUM;
	s.maxGroupNum = MAX_GROUP_NUM;
	s.stepsNum = 256;
	s.bounces = 1;
	s.epsilon = 0.02f;
	s.collisionThreshold = 0.001f;
	s.shadowCollisionThreshold = 0.001f;
	s.backStepK = 100.0f;
	s.maxTraceDist = 10.0f;
	s.sunSize = 1.0f;

	startTime(); // debug init
	initUmka();
	umkaCall(umka, umkaStartFunc, 0, NULL, NULL);
	initOGL(s);
	createScene();

	dprint("INIT DONE");
	dprint("starting main loop");

	float lastTime = 0.0f;
	float frameTime = 0.0f;
	unsigned int frameCount = 0;

	while (!glfwWindowShouldClose(gl->window)) {
		frameCount++;

		updateInput();
		umkaCall(umka, umkaUpdateFunc, 0, NULL, NULL);
		updateScene();

		renderOGL();

		deltaTime = (float)glfwGetTime() - lastTime;
		lastTime = (float)glfwGetTime();

		frameTime += deltaTime;

		if (frameTime > 1.0f) {
			//printf("FPS: %d, MSPF: %.4f\n", frameCount, 1000.0f / (float)frameCount);
			frameCount = 0;
			frameTime = 0.0f;
		}
	}

	freeObjects();
	umkaFree(umka);
	dprint("exiting GL");
	exitOGL();
	dprint("return 0;");
	return 0;
}

void stop() { glfwSetWindowShouldClose(gl->window, GL_TRUE); }
void die()  { exit(1); }
