#include "bindings.h"
#include "vector.h"
#include "sceneapi.h"

void umkaBind(void* umka) {
	umkaAddFunc(umka, "SetShpClr", &umSetShapeClr);

	umkaAddFunc(umka, "CreateSphere",   &umCreateSphere);
	umkaAddFunc(umka, "CreateCube",     &umCreateCube);
	umkaAddFunc(umka, "CreateTorus",    &umCreateTorus);
	umkaAddFunc(umka, "CreateCapsule",  &umCreateCapsule);
	umkaAddFunc(umka, "CreateCylinder", &umCreateCylinder);
	umkaAddFunc(umka, "CreateCCone",    &umCreateCCone);

	umkaAddFunc(umka, "CreateGroup",    &umCreateGroup);
}


#pragma GCC diagnostic ignored "-Wunused-parameter"
void umSetShapeClr(UmkaStackSlot* P, UmkaStackSlot* R) {
	int shpID = (int)P[3].intVal;
	float r = (float)P[2].realVal;
	float g = (float)P[1].realVal;
	float b = (float)P[0].realVal;

	setShapeClr(shpID, v3(r, g, b));
}

void umCreateSphere(UmkaStackSlot* P, UmkaStackSlot* R) {
	float x = (float)P[7].realVal;
	float y = (float)P[6].realVal;
	float z = (float)P[5].realVal;

	float r = (float)P[4].realVal;
	float g = (float)P[3].realVal;
	float b = (float)P[2].realVal;

	float rv = (float)P[1].realVal;
	float rd = (float)P[0].realVal;

	int i = createSphere(v3(x, y, z), v3(r, g, b), rv, rd);

	R[0].intVal = i;
}

void umCreateCube(UmkaStackSlot* P, UmkaStackSlot* R) {
	float x = (float)P[9].realVal;
	float y = (float)P[8].realVal;
	float z = (float)P[7].realVal;

	float r = (float)P[6].realVal;
	float g = (float)P[5].realVal;
	float b = (float)P[4].realVal;

	float rv = (float)P[3].realVal;

	float sx  = (float)P[2].realVal;
	float sy  = (float)P[1].realVal;
	float sz  = (float)P[0].realVal;

	int i = createCube(v3(x, y, z), v3(r, g, b), rv, v3(sx, sy, sz));

	R[0].intVal = i;
}

void umCreateTorus(UmkaStackSlot* P, UmkaStackSlot* R) {
	float x = (float)P[8].realVal;
	float y = (float)P[7].realVal;
	float z = (float)P[6].realVal;

	float r = (float)P[5].realVal;
	float g = (float)P[4].realVal;
	float b = (float)P[3].realVal;

	float rv = (float)P[2].realVal;

	float r1 = (float)P[1].realVal;
	float r2 = (float)P[0].realVal;

	int i = createTorus(v3(x, y, z), v3(r, g, b), rv, r1, r2);

	R[0].intVal = i;
}

void umCreateCapsule(UmkaStackSlot* P, UmkaStackSlot* R) {
	float x = (float)P[10].realVal;
	float y = (float)P[9 ].realVal;
	float z = (float)P[8 ].realVal;

	float r = (float)P[7].realVal;
	float g = (float)P[6].realVal;
	float b = (float)P[5].realVal;

	float rv = (float)P[4].realVal;

	float ex = (float)P[3].realVal;
	float ey = (float)P[2].realVal;
	float ez = (float)P[1].realVal;

	float rd = (float)P[0].realVal;

	int i = createCapsule(v3(x, y, z), v3(r, g, b), rv, v3(ex, ey, ez), rd);

	R[0].intVal = i;
}

void umCreateCylinder(UmkaStackSlot* P, UmkaStackSlot* R) {
	float x = (float)P[10].realVal;
	float y = (float)P[9 ].realVal;
	float z = (float)P[8 ].realVal;

	float r = (float)P[7].realVal;
	float g = (float)P[6].realVal;
	float b = (float)P[5].realVal;

	float rv = (float)P[4].realVal;

	float ex = (float)P[3].realVal;
	float ey = (float)P[2].realVal;
	float ez = (float)P[1].realVal;

	float rd = (float)P[0].realVal;

	int i = createCylinder(v3(x, y, z), v3(r, g, b), rv, v3(ex, ey, ez), rd);

	R[0].intVal = i;
}

void umCreateCCone(UmkaStackSlot* P, UmkaStackSlot* R) {
	float x = (float)P[11].realVal;
	float y = (float)P[10].realVal;
	float z = (float)P[9 ].realVal;

	float r = (float)P[8].realVal;
	float g = (float)P[7].realVal;
	float b = (float)P[6].realVal;

	float rv = (float)P[5].realVal;

	float ex = (float)P[4].realVal;
	float ey = (float)P[3].realVal;
	float ez = (float)P[2].realVal;

	float r1 = (float)P[1].realVal;
	float r2 = (float)P[0].realVal;

	int i = createCCone(v3(x, y, z), v3(r, g, b), rv, v3(ex, ey, ez), r1, r2);

	R[0].intVal = i;
}

void umCreateGroup(UmkaStackSlot* P, UmkaStackSlot* R) {
	int   a = (int)P[3].intVal;
	int   b = (int)P[2].intVal;
	OperationType op = (OperationType)P[1].intVal;
	float k = (float)P[0].realVal;

	int i = createGroup(a, b, op, k);

	R[0].intVal = i;
}
