#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#include "debug.h"
#include "main.h"
#include "sceneio.h"
#include "settings.h"
#include "umka.h"

extern GL* gl;
extern void* umka;
float deltaTime = 0.0f;
extern int umkaStartFunc, umkaUpdateFunc;

int main() {
	printf("\n\n====================================\n\n\n");

	startTime(); // debug init
	initOGL();
	initUmka();
	createScene();

	dprint("INIT DONE");
	dprint("starting main loop");

	float lastTime = 0.0f;
	//float frameTime = 0.0f;
	//unsigned int frameCount = 0;

	umkaCall(umka, umkaStartFunc, 0, NULL, NULL);

	while (!glfwWindowShouldClose(gl->window)) {
		//frameCount++;

		updateInput();
		umkaCall(umka, umkaUpdateFunc, 0, NULL, NULL);
		updateScene();
		renderOGL();

		deltaTime = (float)glfwGetTime() - lastTime;
		lastTime = (float)glfwGetTime();

		/*frameTime += deltaTime;

		if (frameTime > 1.0f) {
			printf("frameCount: %d\n", frameCount);
			frameCount = 0;
			frameTime = 0.0f;
		}*/
	}

	umkaFree(umka);
	dprint("exiting GL");
	exitOGL();
	dprint("return 0;");
	return 0;
}

void stop() { glfwSetWindowShouldClose(gl->window, GL_TRUE); }
void die()  { exit(1); }
