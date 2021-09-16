
#include "portal.h"
#include "alloc.h"

Portal* portal_new(Vector3 pos, Vector3 dir, Vector3 up, Vector2 scl) {
	Portal *p = xmalloc(sizeof(Portal));

	p->pos = pos;
	p->dir = dir;
	p->up  = up;
	p->scl = scl;

	p->link = NULL;

	return p;
}

float* portal_write_bulk(Portal **p, int count) {
	float* arr = xmalloc(sizeof(float) * PORTAL_SIZE * count);

	for (int i = 0; i < count; i++) {
		int j = i * PORTAL_SIZE;
		arr[j + 0] = p[i]->pos.x; arr[j +  1] = p[i]->pos.y; arr[j +  2] = p[i]->pos.z;
		arr[j + 3] = p[i]->dir.x; arr[j +  4] = p[i]->dir.y; arr[j +  5] = p[i]->dir.z;
		arr[j + 6] = p[i]->up .x; arr[j +  7] = p[i]->up .y; arr[j +  8] = p[i]->up .z;
		arr[j + 9] = p[i]->scl.x; arr[j + 10] = p[i]->scl.y; arr[j + 11] = 0.0f;
	}

	return arr;
}

