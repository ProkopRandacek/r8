#include <stdlib.h>

#include "sphere.h"
#include "alloc.h"

Shape* sphere_new(Vector3 pos, Vector4 clr, float r) {
	Shape *s = xmalloc(sizeof(Shape));
	s->type = stPRIMITIVE;

	s->p.type = ptSPHERE;

	s->p.d[0] = pos.x; s->p.d[1] = pos.y; s->p.d[2] = pos.z;
	s->p.d[3] = clr.x; s->p.d[4] = clr.y; s->p.d[5] = clr.z; s->p.d[6] = clr.w;
	s->p.d[10] = r;

	s->p.d[7] = 0.0f;
	s->p.d[8] = 0.0f;
	s->p.d[9] = 0.0f;
	s->p.d[11] = 0.0f;

	return s;
}

