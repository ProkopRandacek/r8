#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "serialization.h"
#include "scene.h"
#include "../settings.h"
#include "../opengl.h"
#include "../shader/shader.h"

extern GL* gl;

Scene* scenes[MAX_SCENE_NUM];
int sceneChanged = 0;
unsigned int bindedScene = 0;

Scene* scene(int maxShapeNum, int maxGroupNum, int stepsNum, int bounces, float maxTraceDist, float sunSize) {
	Scene* s = malloc(sizeof(Scene));
	s->shapeMask = 32767; // all mask
	s->maxShapeNum = maxShapeNum;
	s->maxGroupNum = maxGroupNum;
	s->shapes = malloc(sizeof(Shape     *) * (long unsigned int)maxShapeNum);
	s->groups = malloc(sizeof(ShapeGroup*) * (long unsigned int)maxGroupNum);

	memset(s->shapes, 0, sizeof(Shape     *) * (long unsigned int)maxShapeNum);
	memset(s->groups, 0, sizeof(ShapeGroup*) * (long unsigned int)maxGroupNum);

	s->freeGrpIndex = 0;
	s->freeShpIndex = 0;
	s->stepsNum = stepsNum;
	s->bounces = bounces;
	s->epsilon = 0.02f;
	s->collisionThreshold = 0.001f;
	s->shadowCollisionThreshold = 0.001f;
	s->backStepK = 100.0f;
	s->maxTraceDist = maxTraceDist;
	s->sunSize = sunSize;

	// Camera
	s->cam = malloc(sizeof(Camera));
	cmr(s->cam, v3(0.0f, 0.0f, -4.0f), vNorm(vDir(v3(0.0f, 0.0f, -4.0f), v3(0.0f, 0.0f, 0.0f))), 0.0f);

	return s;
}

void sendCamera() {
	float camFloats[15];
	cam2floats(scenes[bindedScene]->cam, camFloats);
	shdSetVec3Array("cam", 5 * 3, camFloats);
}

void sendObjects() {
	float s[SHAPE_SIZE * scenes[bindedScene]->shapeNum];
	shapes2floats(s, (unsigned int)scenes[bindedScene]->shapeNum, scenes[bindedScene]->shapes);
	shdSetFloatArray("rawShapes" , (int)(SHAPE_SIZE * scenes[bindedScene]->shapeNum), s);
}

void updateScene() {
	// Light temporary static here
	float f[] = {1.0f, 1.0f, -1.0f};
	shdSetVec3Array("lightPos", 1, f);

	// Camera needs to be updated if the window resolution changes
	cmr(scenes[bindedScene]->cam, v3(0.0f, 0.0f, -4.0f), vNorm(vDir(v3(0.0f, 0.0f, -4.0f), v3(0.0f, 0.0f, 0.0f))), 0.0f);
	sendCamera();
	if (sceneChanged == 1) {
		sendObjects();
		sceneChanged = 0;
	}
}

