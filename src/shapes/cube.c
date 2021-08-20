#include <stdlib.h>

#include "cube.h"

Shape* cube_new(Vector3 pos, Vector4 clr, Vector3 scl) {
	Shape *s = malloc(sizeof(Shape));
	s->type = stPRIMITIVE;

	s->p.type = ptCUBE;

	s->p.d[0] = pos.x; s->p.d[1] = pos.y; s->p.d[2] = pos.z;
	s->p.d[3] = clr.x; s->p.d[4] = clr.y; s->p.d[5] = clr.z; s->p.d[6] = clr.w;
	s->p.d[7] = scl.x; s->p.d[8] = scl.y; s->p.d[9] = scl.z;

	return s;
}

