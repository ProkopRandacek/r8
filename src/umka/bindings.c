#include "bindings.h"
#include "../vector.h"
#include "../scene/sceneapi.h"

#include "../scene/camera.h"

extern Camera* cam;
extern vec3 lightPos;

void umkaBind(void* umka) {
	umkaAddFunc(umka, "SetShpClr",      &umSetShapeClr);
	umkaAddFunc(umka, "CreateSphere",   &umCreateSphere);
	umkaAddFunc(umka, "CreateCube",     &umCreateCube);
	umkaAddFunc(umka, "CreateTorus",    &umCreateTorus);
	umkaAddFunc(umka, "CreateCapsule",  &umCreateCapsule);
	umkaAddFunc(umka, "CreateCylinder", &umCreateCylinder);
	umkaAddFunc(umka, "CreateCCone",    &umCreateCCone);
	umkaAddFunc(umka, "CreateGroup",    &umCreateGroup);

	umkaAddFunc(umka, "GetCamPos", &umGetCamPos);
	umkaAddFunc(umka, "GetCamDir", &umGetCamDir);

	umkaAddFunc(umka, "SetLightPos", &umSetLightPos);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void umSetShapeClr(UmkaStackSlot* p, UmkaStackSlot* r) {
	int shpID =   (int)p[1].intVal;
	vec3* clr = (vec3*)p[0].ptrVal;

	setShapeClr(shpID, *clr);
}

void umCreateSphere(UmkaStackSlot* p, UmkaStackSlot* r) {
	vec3* pos = (vec3*)p[3].ptrVal;
	vec3* clr = (vec3*)p[2].ptrVal;
	float rv  = (float)p[1].realVal;
	float rd  = (float)p[0].realVal;

	int i = createSphere(*pos, *clr, rv, rd);

	r[0].intVal = i;
}

void umCreateCube(UmkaStackSlot* p, UmkaStackSlot* r) {
	vec3* pos = (vec3*)p[3].ptrVal;
	vec3* clr = (vec3*)p[2].ptrVal;
	float rv  = (float)p[1].realVal;
	vec3* scl = (vec3*)p[0].ptrVal;

	int i = createCube(*pos, *clr, rv, *scl);

	r[0].intVal = i;
}

void umCreateTorus(UmkaStackSlot* p, UmkaStackSlot* r) {
	vec3* pos = (vec3*)p[4].ptrVal;
	vec3* clr = (vec3*)p[3].ptrVal;
	float rv  = (float)p[2].realVal;
	float r1  = (float)p[1].realVal;
	float r2  = (float)p[0].realVal;

	int i = createTorus(*pos, *clr, rv, r1, r2);

	r[0].intVal = i;
}

void umCreateCapsule(UmkaStackSlot* p, UmkaStackSlot* r) {
	vec3* pos = (vec3*)p[4].ptrVal;
	vec3* clr = (vec3*)p[3].ptrVal;
	float rv  = (float)p[2].realVal;
	vec3* end = (vec3*)p[1].ptrVal;
	float rd  = (float)p[0].realVal;

	int i = createCapsule(*pos, *clr, rv, *end, rd);

	r[0].intVal = i;
}

void umCreateCylinder(UmkaStackSlot* p, UmkaStackSlot* r) {
	vec3* pos = (vec3*)p[4].ptrVal;
	vec3* clr = (vec3*)p[3].ptrVal;
	float rv  = (float)p[2].realVal;
	vec3* end = (vec3*)p[1].ptrVal;
	float rd  = (float)p[0].realVal;

	int i = createCylinder(*pos, *clr, rv, *end, rd);

	r[0].intVal = i;
}

void umCreateCCone(UmkaStackSlot* p, UmkaStackSlot* r) {
	vec3* pos = (vec3*)p[5].ptrVal;
	vec3* clr = (vec3*)p[4].ptrVal;
	float rv  = (float)p[3].realVal;
	vec3* end = (vec3*)p[2].ptrVal;
	float r1  = (float)p[1].realVal;
	float r2  = (float)p[0].realVal;

	int i = createCCone(*pos, *clr, rv, *end, r1, r2);

	r[0].intVal = i;
}

void umCreateGroup(UmkaStackSlot* p, UmkaStackSlot* r) {
	int           a  = (int)          p[3].intVal;
	int           b  = (int)          p[2].intVal;
	OperationType op = (OperationType)p[1].intVal;
	float         k  = (float)        p[0].realVal;

	int i = createGroup(a, b, op, k);

	r[0].intVal = i;
}

void umGetCamPos(UmkaStackSlot* p, UmkaStackSlot* r) { r[0].ptrVal = (intptr_t)&cam->pos;     }
void umGetCamDir(UmkaStackSlot* p, UmkaStackSlot* r) { r[0].ptrVal = (intptr_t)&cam->forward; }

void umSetLightPos(UmkaStackSlot* p, UmkaStackSlot* r) {
	vec3* t = (vec3*)p[0].ptrVal;
	lightPos = *t;
}

