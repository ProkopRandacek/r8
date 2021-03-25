#include <stdlib.h>

#include "vector.h"
#include "shapes.h"

inline Sphere* sph(vec3 pos, vec3 clr, float radius, float rv) {
	Sphere* s = malloc(sizeof(Sphere));
	s->pos = pos;
	s->clr = clr;
	s->radius = radius;
	s->rv = rv;
	return s;
}

inline Cube* cube(vec3 pos, vec3 clr, vec3 scale, float roundEdge, float rv) {
	Cube* c = malloc(sizeof(Cube));
	c->pos = pos;
	c->clr = clr;
	c->scale = scale;
	c->roundEdge = roundEdge;
	c->rv = rv;
	return c;
}

/*BoxFrame* frame(vec3 pos, vec3 clr, vec3 scale, float width) {
	BoxFrame* f = malloc(sizeof(BoxFrame));
	f->pos = pos;
	f->clr = clr;
	f->scale = scale;
	f->width = width;
	return f;
}*/

inline Torus* tor(vec3 pos, vec3 clr, float radius, float fatness, float rv) {
	Torus* t = malloc(sizeof(Torus));
	t->pos = pos;
	t->clr = clr;
	t->radius = radius;
	t->fatness = fatness;
	t->rv = rv;
	return t;
}

/*CTorus* ctor(vec3 pos, vec3 clr, float scX, float scY, float ra, float rb) {
	CTorus* t = malloc(sizeof(CTorus));
	t->pos = pos;
	t->clr = clr;
	t->scX = scX;
	t->scY = scY;
	t->ra = ra;
	t->rb = rb;
	return t;
}

Link* lik(vec3 pos, vec3 clr, float innerR, float outerR, float length) {
	Link* l = malloc(sizeof(Link));
	l->pos = pos;
	l->clr = clr;
	l->innerR = innerR;
	l->outerR = outerR;
	l->length = length;
	return l;
}

Plane* pln(vec3 pos, vec3 clr, vec3 n, float h) {
	Plane* p = malloc(sizeof(Plane));
	p->pos = pos;
	p->clr = clr;
	p->n = n;
	p->h = h;
	return p;
}

HexPrism* xprism(vec3 pos, vec3 clr, float h, float r) {
	HexPrism* x = malloc(sizeof(HexPrism));
	x->pos = pos;
	x->clr = clr;
	x->height = h;
	x->radius = r;
	return x;
}

TriPrism* tprism(vec3 pos, vec3 clr, float h, float r) {
	TriPrism* t = malloc(sizeof(TriPrism));
	t->pos = pos;
	t->clr = clr;
	t->height = h;
	t->radius = r;
	return t;
}*/

inline Capsule* caps(vec3 start, vec3 end, vec3 clr, float r, float rv) {
	Capsule* c = malloc(sizeof(Capsule));
	c->start = start;
	c->end = end;
	c->clr = clr;
	c->radius = r;
	c->rv = rv;
	return c;
}

inline Cylinder* cyl(vec3 start, vec3 end, vec3 clr, float r, float rv) {
	Cylinder* c = malloc(sizeof(Cylinder));
	c->start = start;
	c->end = end;
	c->clr = clr;
	c->radius = r;
	c->rv = rv;
	return c;
}

inline CCone* ccone(vec3 start, vec3 end, vec3 clr, float startR, float endR, float rv) {
	CCone* r = malloc(sizeof(CCone));
	r->start = start;
	r->end = end;
	r->clr = clr;
	r->startR = startR;
	r->endR = endR;
	r->rv = rv;
	return r;
}

/*Pyramid* pyr(vec3 pos, vec3 clr, float h) {
	Pyramid* r = malloc(sizeof(Pyramid));
	r->pos = pos;
	r->clr = clr;
	r->h = h;
	return r;
}

Triangle* tri(vec3 a, vec3 b, vec3 c, vec3 clr) {
	Triangle* t = malloc(sizeof(Triangle));
	t->a = a;
	t->b = b;
	t->c = c;
	t->clr = clr;
	return t;
}

Quad* quad(vec3 a, vec3 b, vec3 c, vec3 d, vec3 clr) {
	Quad* q = malloc(sizeof(Quad));
	q->a = a;
	q->b = b;
	q->c = c;
	q->d = d;
	q->clr = clr;
	return q;
}*/


inline Primitive* prmv(ShapeType type, void* shape) {
	Primitive* p = malloc(sizeof(Primitive));
	p->type = type;
	p->shape = shape;
	return p;
}

inline ShapeGroup* group(ShapeType ta, int a, ShapeType tb, int b, OperationType op, float k) {
	ShapeGroup* sg = malloc(sizeof(ShapeGroup));
	sg->a = a;
	sg->b = b;
	sg->ta = ta;
	sg->tb = tb;
	sg->op = op;
	sg->k = k;
	return sg;
}
