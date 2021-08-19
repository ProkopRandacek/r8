#include <stdlib.h>

#include "cube.h"

inline Primitive* cube_new(Vector3 pos, Vector4 clr, Vector3 scl) {
	Primitive *p = malloc(sizeof(Primitive));

	p->type = ptCUBE;

	p->d[0] = pos.x; p->d[1] = pos.y; p->d[2] = pos.z;
	p->d[3] = clr.x; p->d[4] = clr.y; p->d[5] = clr.z; p->d[6] = clr.w;
	p->d[7] = scl.x; p->d[8] = scl.y; p->d[9] = scl.z;

	return p;
}

