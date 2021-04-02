#include <stdlib.h>

#include "../vector.h"
#include "shapes.h"

inline Shape* sph(vec3 pos, vec3 clr, float radius, float rv) {
	Sphere* s = malloc(sizeof(Sphere));
	s->radius = radius;

	return shape(SPHERE, s, pos, clr, rv);
}

inline Shape* cube(vec3 pos, vec3 clr, vec3 scale, float roundEdge, float rv) {
	Cube* c = malloc(sizeof(Cube));
	c->scale = scale;
	c->roundEdge = roundEdge;

	return shape(CUBE, c, pos, clr, rv);
}

inline Shape* tor(vec3 pos, vec3 clr, float radius, float fatness, float rv) {
	Torus* t = malloc(sizeof(Torus));
	t->radius = radius;
	t->fatness = fatness;

	return shape(TORUS, t, pos, clr, rv);
}

inline Shape* caps(vec3 start, vec3 end, vec3 clr, float r, float rv) {
	Capsule* c = malloc(sizeof(Capsule));
	c->end = end;
	c->radius = r;

	return shape(CAPSULE, c, start, clr, rv);
}

inline Shape* cyl(vec3 start, vec3 end, vec3 clr, float r, float rv) {
	Cylinder* c = malloc(sizeof(Cylinder));
	c->end = end;
	c->radius = r;

	return shape(CYLINDER, c, start, clr, rv);
}

inline Shape* ccone(vec3 start, vec3 end, vec3 clr, float startR, float endR, float rv) {
	CCone* r = malloc(sizeof(CCone));
	r->end = end;
	r->startR = startR;
	r->endR = endR;

	return shape(CCONE, r, start, clr, rv);
}

inline Shape* shape(ShapeType type, void* shape, vec3 pos, vec3 clr, float rv) {
	Shape* p = malloc(sizeof(Shape));
	p->type = type;
	p->shape = shape;
	p->pos = pos;
	p->clr = clr;
	p->rv = rv;
	return p;
}

inline ShapeGroup* group(int a, int b, OperationType op, float k) {
	ShapeGroup* sg = malloc(sizeof(ShapeGroup));
	sg->a = a;
	sg->b = b;
	sg->op = op;
	sg->k = k;
	return sg;
}
