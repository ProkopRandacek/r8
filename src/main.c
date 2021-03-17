#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#include "debug.h"
#include "main.h"
#include "sceneio.h"


extern GL* gl;

float deltaTime = 0.0f;

int main() {
	printf("\n\n====================================\n\n\n");

	printf("Compiled against GLFW %i.%i.%i\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Running against GLFW %i.%i.%i\n", major, minor, revision);

	startTime(); // debug init

	initOGL();

	glfwSetErrorCallback(onError);
	glfwSetKeyCallback(gl->window, onKey);
	glfwSetFramebufferSizeCallback(gl->window, resize);
	glfwSetKeyCallback(gl->window, onKey);

	createScene();

	glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	dprint("INIT DONE");

	float lastTime = 0.0f;

	dprint("starting main loop");

	/*readScene();
	stop();*/

	float frameTime = 0.0f;
	unsigned int frameCount = 0;

	while (!glfwWindowShouldClose(gl->window)) {
		frameCount++;

		updateInput();
		updateScene();
		renderOGL();

		deltaTime = (float)glfwGetTime() - lastTime;
		lastTime = (float)glfwGetTime();

		frameTime += deltaTime;


		if (frameTime > 1.0f) {
			if (frameCount < 60) { // This optimalization has no effect on the fps :(
				char msg[149];
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

// Callback funcs
void onError(int error, const char* description) {
	printf("Glfw error %d: %s\n", error, description);
}

void resize(GLFWwindow* window, int width, int height) {
	if (window != gl->window) return;

	glViewport(0, 0, width, height);
	shdSetIVec2(gl->s, "resolution", width, height);
	float k = 1000.0f;
	setWH((float)width / k, (float)height / k);
}
