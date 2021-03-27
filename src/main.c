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
#include "bindings.h"
#include "../submodules/umka/src/umka_api.h"


extern GL* gl;
void* umka;

float deltaTime = 0.0f;

int umkaStartFunc, umkaUpdateFunc;

int main() {
	printf("\n\n====================================\n\n\n");
	printf("Compiled against GLFW %i.%i.%i\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Running against GLFW %i.%i.%i\n", major, minor, revision);

	startTime(); // debug init
	initOGL();
	createScene();

	// === umka
	dprint("UMKA START");
	umka = umkaAlloc();
	int umkaOK;

	umkaOK = umkaInit(umka, "scripts/game.um", NULL, 1024 * 1024, 1024 * 1024, 0, NULL);
	if (!umkaOK) { eprint("umka init failed"); }
	else         { dprint("UMKA - init successful"); }

	umkaBind(umka);
	dprint("UMKA - bind done");

	umkaOK = umkaCompile(umka);
	if (!umkaOK) {
		UmkaError error;
		umkaGetError(umka, &error);
		char msg[1072];
		sprintf(msg, "Umka compile error %s (%d, %d): %s\n", error.fileName, error.line, error.pos, error.msg);
		eprint(msg);
	}
	dprint("UMKA - compiled successfully");

	umkaStartFunc  = umkaGetFunc(umka, NULL, "start");
	umkaUpdateFunc = umkaGetFunc(umka, NULL, "update");

	dprint("UMKA DONE");



	dprint("INIT DONE");
	dprint("starting main loop");

	float lastTime = 0.0f;
	float frameTime = 0.0f;
	unsigned int frameCount = 0;

	umkaCall(umka, umkaStartFunc, 0, NULL, NULL);

	while (!glfwWindowShouldClose(gl->window)) {
		frameCount++;

		updateInput();
		updateScene();
		renderOGL();

		deltaTime = (float)glfwGetTime() - lastTime;
		lastTime = (float)glfwGetTime();

		frameTime += deltaTime;

		if (frameTime > 1.0f) {
			if (frameCount < WANTED_FPS) {
				char msg[59];
				sprintf(msg, "Low fps: %d", frameCount);
				dprint(msg);
			}
			frameCount = 0;
			frameTime = 0.0f;
		}
	}

	dprint("exiting GL");
	exitOGL();
	dprint("return 0;");
	return 0;
}

void stop() { glfwSetWindowShouldClose(gl->window, GL_TRUE); }
void die() { exit(1); }
