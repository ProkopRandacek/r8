#include <string.h>

#include "cube.h"
#include "alloc.h"

Shape *group_new(char* name, Shape *a, Shape *b, GroupType gt, float k) {
	Shape *s = xmalloc(sizeof(Shape));
	s->type = stGROUP;

	strncpy(s->name, name, SHAPE_NAME_LEN - 1);
	s->name[SHAPE_NAME_LEN - 1] = '\0'; // Make sure that the string is zero terminated.

	s->g.collapsed = false;

	s->g.a = a;
	s->g.b = b;
	s->g.type = gt;
	s->g.k = k;

	return s;
}
