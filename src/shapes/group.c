#include <stdlib.h>

#include "cube.h"

Shape *group_new(Shape *a, Shape *b, GroupType gt, float k) {
	Shape *s = malloc(sizeof(Shape));
	s->type = stGROUP;

	s->g.a = a;
	s->g.b = b;
	s->g.type = gt;
	s->g.k = k;

	return s;
}
