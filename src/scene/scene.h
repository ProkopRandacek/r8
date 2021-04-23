// vim: filetype=c
#ifndef SCENE_H
#define SCENE_H

#include "shapes.h"
#include "camera.h"

typedef struct Scene {
	ShapeMask shapeMask;
	Camera* cam;
	Shape      **shapes; // array of pointers to shape
	ShapeGroup **groups; // array of pointers to group
	int maxShapeNum;
	int maxGroupNum;
	int shapeNum;
	int groupNum;
	int stepsNum;
	int bounces;
	int freeGrpIndex;
	int freeShpIndex;
	float epsilon;
	float collisionThreshold;
	float shadowCollisionThreshold;
	float backStepK;
	float maxTraceDist;
	float sunSize;
} Scene;

Scene* scene(int maxShapeNum, int maxGroupNum, int stepsNum, int bounces, float maxTraceDist, float sunSize);

void updateScene(void);
void sendCamera(void);
void createObjects(void);
void sendObjects(void);

#endif
