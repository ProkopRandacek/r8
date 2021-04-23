#include <stdio.h>
#include "debug.h"
#include "main.h"
#include "umka/umka.h"
#include "scene/scene.h"
#include "opengl.h"

extern GL* gl;
extern void* umka;
extern int umkaStartFunc, umkaUpdateFunc;

float dt;

int main() {
	printf("\nBuild name: %s\nI'm running on ", BUILD_NAME);
#ifdef _WIN32
	printf("Windows\n\n");
#else
	printf("Linux\n\n");
#endif

	// INIT STUFF
	//============
	startDebugTime();
	dprint("INIT START");
	initOGL();
	initUmka();
	dprint("INIT DONE");

	// CALL UMKA START FUNC
	//======================
	dprint("CALLING UMKA START FUNC");
	umkaCall(umka, umkaStartFunc, 0, NULL, NULL);

	// FPS counting variables
	//========================
	float lt = 0.0f, ft = 0.0f;
	dt = 0.0f;
	int   fc = 0;

	dprint("ENTERING RENDER LOOP");
	while (!glfwWindowShouldClose(gl->window)) {
		// FPS and delta time counting
		//=============================
		fc++;
		dt = (float)glfwGetTime() - lt;
		lt = (float)glfwGetTime();
		ft += dt;
		if (ft > 1.0f) {
			//printf("FPS: %d, MSPF: %.4f\n", fc, 1000.0f / (float)fc);
			fc = 0;
			ft = 0;
		}

		// UPDATE & RENDER
		//=================
		umkaCall(umka, umkaUpdateFunc, 0, NULL, NULL);
		updateScene();
		renderOGL();
	}

	// EXIT
	//======
	dprint("Exiting");
	exitUmka();
	exitOGL();
	return 0;
}

