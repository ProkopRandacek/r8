#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "../opengl.h"
#include "scene.h"
#include "serialization.h"
#include "../settings.h"
#include "sceneapi.h"

extern GL* gl;

Shape*      shapes[MAX_SHAPE_NUM];
ShapeGroup* groups[MAX_GROUP_NUM];

int shapeNum = 0;
int groupNum = 0;
int sceneChanged = 0;

extern Camera* cam;

void createScene() {
	vec3 campos = v3(0.0f, 5.0f, -10.0f);
	cmr(campos, vNorm(vDir(campos, v3(0.0f, 5.0f, 0.0f))), 0.0f, 0.01f, 0.01f);
	float lsPos[] = {5.0f, 2.0f, 5.0f};
	shdSetVec3Array(gl->s, "lightPos", 1, lsPos);
}

void sendCamera() {
	float camFloats[15];
	cam2floats(cam, camFloats);
	shdSetVec3Array(gl->s, "cam", 5 * 3, camFloats);
}

void freeObjects() {
	for (int i = 0; i < shapeNum; i++) { free(shapes[i]->shape); free(shapes[i]); }
	for (int i = 0; i < groupNum; i++) { free(groups[i]); }
	free(cam);
}

void sendObjects() {
	float s[SHAPE_SIZE * shapeNum];
	shapes2floats(s, (unsigned int)shapeNum, shapes);
	shdSetFloatArray(gl->s, "rawShapes" , (int)(SHAPE_SIZE * shapeNum), s);
}

void updateScene() {
	sendCamera();
	if (sceneChanged == 1) {
		sendObjects();
		sceneChanged = 0;
	}
}
