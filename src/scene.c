#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "opengl.h"
#include "scene.h"
#include "serialization.h"
#include "settings.h"
#include "sceneapi.h"


extern GL* gl;

Shape*      shapes[SHAPE_NUM];
ShapeGroup* groups[GROUP_NUM];

// these are global so other object can be created relative to them
Camera cam;

void createScene() {
	createCamera();
	sendCamera();
	createLight();
	createObjects();
}

void createCamera() {
	vec3 campos = v3(0.0f, 5.0f, -10.0f);
	cam = cmr(campos, vNorm(vDir(campos, v3(0.0f, 5.0f, 0.0f))), 0.0f, 0.01f, 0.01f);
}

void sendCamera() {
	float camFloats[15];
	cam2floats(cam, camFloats);

	shdSetVec3Array(gl->s, "cam", 5 * 3, camFloats);
}

void createLight() {
	float lsPos[] = {5.0f, 10.0f, 5.0f};

	shdSetVec3Array(gl->s, "lightPos", 1, lsPos);
}

void createObjects() {
	// Primitives

	int head = createSphere(v3(0.5f, 2.1f, 0.5f), v3f(0.7f), 0.0f, 0.3f);
	int ring = createTorus (v3(0.5f, 1.9f, 0.5f), v3f(0.7f), 0.0f, 0.25f, 0.09f);
	int body = createCCone (v3(0.5f, 2.2f, 0.5f), v3f(0.7f), 0.0f, v3(0.5f, 1.3f, 0.5f), 0.17f, 0.3f);
	int base = createCCone (v3(0.5f, 1.3f, 0.5f), v3f(0.7f), 0.0f, v3(0.5f, 1.0f, 0.5f), 0.4f, 0.4f);

	// Groups
	// the top group is a sphere that is on index 0 in shapes array and a torus that is on index 1. operation between then is normal
	ShapeGroup* sgTop    = group(SPHERE, head, TORUS, ring, NORMAL, 0.0f);
	ShapeGroup* sgBottom = group(CCONE,  body, CCONE, base, BLEND, 0.2f);
	ShapeGroup* sgROOT   = group(GROUP,  0, GROUP, 1, BLEND, 0.02f);

	groups[0] = sgTop;
	groups[1] = sgBottom;
	groups[2] = sgROOT;

	sendObjects();
}

void freeObjects() {
	for (unsigned int i = 0; i < SHAPE_NUM; i++) { free(shapes[i]->shape); free(shapes[i]); }
	for (unsigned int i = 0; i < GROUP_NUM; i++) { free(groups[i]); }
}

void sendObjects() {
	float g[GROUP_SIZE * GROUP_NUM];
	float s[SHAPE_SIZE * SHAPE_NUM];

	shapes2floats(s, SHAPE_NUM, shapes);
	shdSetFloatArray(gl->s, "rawShapes", (int)(SHAPE_SIZE * SHAPE_NUM), s);

	groups2floats(g, GROUP_NUM, groups);
	shdSetFloatArray(gl->s, "rawGroups", (int)(GROUP_SIZE * GROUP_NUM), g);
}

void updateScene() {
	// recreate all object that are supposed to be moving
	sendCamera();

	/*setShapeClr(0, v3f(sinf((float)glfwGetTime() * 3.0f) * 0.5f + 0.5f));
	setShapePos(1, v3f(sinf((float)glfwGetTime() * 3.0f) * 0.5f + 0.5f));
	setShapeRv (3, sinf((float)glfwGetTime() * 3.0f) * 0.5f + 0.5f);*/

	sendObjects();
}
