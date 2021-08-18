#include "sphere.h"

inline Primitive new_sphere(Vector3 pos, Vector4 clr, float r) {
	Primitive p;
	p.type = ptSPHERE;
	p.d[0] = pos.x; p.d[1] = pos.y; p.d[2] = pos.z;
	p.d[3] = clr.x; p.d[4] = clr.y; p.d[5] = clr.z; p.d[6] = clr.w;
	p.d[10] = r;

	return p;
}

