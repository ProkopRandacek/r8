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

int shapeNum = 0;
int groupNum = 0;

Camera cam;

void createScene() {
	createCamera();
	createLight();
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

void freeObjects() {
	for (int i = 0; i < shapeNum; i++) { free(shapes[i]->shape); free(shapes[i]); }
	for (int i = 0; i < groupNum; i++) { free(groups[i]); }
}

void sendObjects() {
	float g[GROUP_SIZE * groupNum];
	float s[SHAPE_SIZE * shapeNum];

	shapes2floats(s, (unsigned int)shapeNum, shapes);
	groups2floats(g, (unsigned int)groupNum, groups);

	shdSetFloatArray(gl->s, "rawShapes", (int)(SHAPE_SIZE * shapeNum), s);
	shdSetFloatArray(gl->s, "rawGroups", (int)(GROUP_SIZE * groupNum), g);
}

void updateScene() {
	sendCamera();
	sendObjects();
}
