#include "../submodules/umka/src/umka_api.h"

#include "bindings.h"

void umkaBind(void* umka) {
	umkaAddFunc(umka, "setShapeClr". &umSetShapeClr);
}


void umSetShapeClr(UmkaStackSlot* p, UmkaStackSlot* r) {
	int shpID = p[3].intVal;
	float r = (float)p[2].realVal;
	float g = (float)p[1].realVal;
	float b = (float)p[0].realVal;

	setShapeClr(shpID, v3(r, g, b));
}
