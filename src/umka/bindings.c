#include "bindings.h"
#include "sceneapi.h"

void umkaBind(void* umka) {
	umkaAddFunc(umka, "SetShpClr", &umSetShapeClr);
	umkaAddFunc(umka, "SetShpPos", &umSetShapePos);
	umkaAddFunc(umka, "SetShpRv",  &umSetShapeRv);

	umkaAddFunc(umka, "CreateSphere",   &umCreateSphere);
	umkaAddFunc(umka, "CreateCube",     &umCreateCube);
	umkaAddFunc(umka, "CreateTorus",    &umCreateTorus);
	umkaAddFunc(umka, "CreateCapsule",  &umCreateCapsule);
	umkaAddFunc(umka, "CreateCylinder", &umCreateCylinder);
	umkaAddFunc(umka, "CreateCCone",    &umCreateCCone);
	umkaAddFunc(umka, "CreateGroup",    &umCreateGroup);
	umkaAddFunc(umka, "GetCamPos",      &umGetCamPos);
	umkaAddFunc(umka, "GetCamDir",      &umGetCamDir);
	//umkaAddFunc(umka, "SetLightPos",    &umSetLightPos);
	umkaAddFunc(umka, "CreateScene",    &umCreateScene);
	umkaAddFunc(umka, "BindScene",      &umBindScene);
	umkaAddFunc(umka, "BindShader",     &umBindShader);
	umkaAddFunc(umka, "CompileScene",   &umCompileScene);
	umkaAddFunc(umka, "SetCam",         &umSetCam);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void umSetShapeClr(UmkaStackSlot* p, UmkaStackSlot* r) { setShapeClr((int)p[1].intVal, *(vec3*)p[0].ptrVal); }
void umSetShapePos(UmkaStackSlot* p, UmkaStackSlot* r) { setShapePos((int)p[1].intVal, *(vec3*)p[0].ptrVal); }
void umSetShapeRv (UmkaStackSlot* p, UmkaStackSlot* r) { setShapeRv ((int)p[1].intVal,  (float)p[0].ptrVal); }

void umSetCam(UmkaStackSlot* p, UmkaStackSlot* r) {
	vec3  pos = *(vec3*)p[2].ptrVal;
	vec3  dir = *(vec3*)p[1].ptrVal;
	float ang =  (float)p[0].ptrVal;
	setCam(pos, dir, ang);
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

void umCreateScene(UmkaStackSlot* p, UmkaStackSlot* r) {
	float sunSize      = (float)p[0].realVal;
	float maxTraceDist = (float)p[1].realVal;
	int   bounces      = (int)  p[2].intVal;
	int   stepsNum     = (int)  p[3].intVal;
	int   maxGroupNum  = (int)  p[4].intVal;
	int   maxShapeNum  = (int)  p[5].intVal;

	int i = createScene(maxShapeNum, maxGroupNum, stepsNum, bounces, maxTraceDist, sunSize);

	r[0].intVal = i;
}

void umCompileScene(UmkaStackSlot* p, UmkaStackSlot* r) { r[0].intVal = compileScene((int)p[0].intVal); }
void umBindScene   (UmkaStackSlot* p, UmkaStackSlot* r) { bindScene((int)p[0].intVal); }
void umBindShader  (UmkaStackSlot* p, UmkaStackSlot* r) { bindShader((unsigned int*)p[0].ptrVal); }

void umGetCamPos(UmkaStackSlot* p, UmkaStackSlot* r) { r[0].ptrVal = (intptr_t)getCamPos(); }
void umGetCamDir(UmkaStackSlot* p, UmkaStackSlot* r) { r[0].ptrVal = (intptr_t)getCamDir(); }
//void umSetLightPos(UmkaStackSlot* p, UmkaStackSlot* r) { moveLight(*(vec3*)p[0].ptrVal); }

#pragma GCC diagnostic pop

