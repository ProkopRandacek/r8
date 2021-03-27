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

Shape*      shapes[MAX_SHAPE_NUM];
ShapeGroup* groups[MAX_GROUP_NUM];

unsigned int shapeNum = 0;
unsigned int groupNum = 0;

// these are global so other object can be created relative to them
Camera cam;

void createScene() {
	createCamera();
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
	/*// Primitives
	int head = createSphere(v3(0.5f, 2.1f, 0.5f), v3f(0.7f), 0.0f, 0.3f);
	int ring = createTorus (v3(0.5f, 1.9f, 0.5f), v3f(0.7f), 0.0f, 0.25f, 0.09f);
	int body = createCCone (v3(0.5f, 2.2f, 0.5f), v3f(0.7f), 0.0f, v3(0.5f, 1.3f, 0.5f), 0.17f, 0.3f);
	int base = createCCone (v3(0.5f, 1.3f, 0.5f), v3f(0.7f), 0.0f, v3(0.5f, 1.0f, 0.5f), 0.4f, 0.4f);

	// Groups
	int top    = createGroup(head, ring,  NORMAL, 0.0f );
	int bottom = createGroup(body, base,  BLEND,  0.2f );
	int root   = createGroup(top, bottom, BLEND,  0.02f);

	sendObjects();*/
}

void freeObjects() {
	for (unsigned int i = 0; i < shapeNum; i++) { free(shapes[i]->shape); free(shapes[i]); }
	for (unsigned int i = 0; i < groupNum; i++) { free(groups[i]); }
}

void sendObjects() {
	float g[GROUP_SIZE * groupNum];
	float s[SHAPE_SIZE * shapeNum];
	//printf("%d/%d, %d/%d\n", shapeNum, MAX_SHAPE_NUM, groupNum, MAX_GROUP_NUM);

	shapes2floats(s, shapeNum, shapes);
	groups2floats(g, groupNum, groups);

	shdSetFloatArray(gl->s, "rawShapes", (int)(SHAPE_SIZE * shapeNum), s);
	shdSetFloatArray(gl->s, "rawGroups", (int)(GROUP_SIZE * groupNum), g);
}

void updateScene() {
	sendCamera();
	sendObjects();
}

