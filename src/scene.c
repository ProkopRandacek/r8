#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "opengl.h"
#include "scene.h"
#include "serialization.h"


extern GL* gl;
extern const unsigned int groupSize, shapeSize;

#define shapeNum 4
#define groupNum 3

Primitive*  shapes[shapeNum];
ShapeGroup* groups[groupNum];

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

	Primitive* head = prmv(SPHERE, (void*) sph  (v3(0.5f, 2.1f, 0.5f), v3(0.9f, 0.0f, 0.9f), 0.3f, 0.0f));
	Primitive* ring = prmv(TORUS,  (void*) tor  (v3(0.5f, 1.9f, 0.5f), v3(0.9f, 0.0f, 0.9f), 0.25f, 0.09f, 0.0f));
	Primitive* body = prmv(CCONE,  (void*) ccone(v3(0.5f, 2.2f, 0.5f), v3(0.5f, 1.3f, 0.5f), v3(0.9f, 0.0f, 0.9f), 0.17f, 0.3f, 0.0f));
	Primitive* base = prmv(CCONE,  (void*) ccone(v3(0.5f, 1.3f, 0.5f), v3(0.5f, 1.0f, 0.5f), v3(0.9f, 0.0f, 0.9f), 0.4f, 0.4f, 0.0f));

	shapes[0] = head;
	shapes[1] = ring;
	shapes[2] = body;
	shapes[3] = base;


	// Groups
	// the top group is a sphere that is on index 0 in shapes array and a torus that is on index 1. operation between then is normal
	ShapeGroup* sgTop    = group(SPHERE, 0, TORUS, 1, NORMAL, 0.0f);
	ShapeGroup* sgBottom = group(CCONE,  2, CCONE, 3, BLEND, 0.2f);
	ShapeGroup* sgROOT   = group(GROUP,  0, GROUP, 1, BLEND, 0.02f);

	groups[0] = sgTop;
	groups[1] = sgBottom;
	groups[2] = sgROOT;

	sendObjects();

	/*shdSetInt(gl->s, "shapeNum", shapeNum);
	shdSetInt(gl->s, "shapeSize", shapeSize);
	shdSetInt(gl->s, "groupNum", groupNum);
	shdSetInt(gl->s, "groupSize", groupSize);*/

	// cleanup
	for (unsigned int i = 0; i < shapeNum; i++) { free(shapes[i]->shape); free(shapes[i]); }
	for (unsigned int i = 0; i < groupNum; i++) { free(groups[i]); }
}

void sendObjects() {
	float g[groupSize * groupNum];
	float s[shapeSize * shapeNum];

	shapes2floats(s, shapeNum, shapes);
	shdSetFloatArray(gl->s, "rawShapes", (int)(shapeSize * shapeNum), s);

	groups2floats(g, groupNum, groups);
	shdSetFloatArray(gl->s, "rawGroups", (int)(groupSize * groupNum), g);
}

void updateScene() {
	// recreate all object that are supposed to be moving
	sendCamera();
}
