#include "shapes.h"

Vector3 prim_get_pos(Primitive *p) {
	return (Vector3){
		p->d[0],
		p->d[1],
		p->d[2]
	};
}

Vector4 prim_get_clr(Primitive *p) {
	return (Vector4){
		p->d[3],
		p->d[4],
		p->d[5],
		p->d[6]
	};
}

float prim_get_rad1(Primitive *p) {
	return p->d[10];
}

float prim_get_rad2(Primitive *p) {
	return p->d[11];
}
