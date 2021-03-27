
#include "bindings.h"
#include "vector.h"
#include "sceneapi.h"

void umkaBind(void* umka) {
	umkaAddFunc(umka, "SetShpClr", &umSetShapeClr);
}


#pragma GCC diagnostic ignored "-Wunused-parameter"
void umSetShapeClr(UmkaStackSlot* p, UmkaStackSlot* r) {
	int shpID = (int)p[3].intVal;
	float R = (float)p[2].realVal;
	float G = (float)p[1].realVal;
	float B = (float)p[0].realVal;

	setShapeClr(shpID, v3(R, G, B));
}
