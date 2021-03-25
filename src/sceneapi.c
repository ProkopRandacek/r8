#include "serialization.h"
#include "sceneapi.h"
#include "settings.h"
#include "debug.h"

#include "settings.h"

int freeShpIndex = 0;
int freeGrpIndex = 0;

extern Shape*      shapes[SHAPE_NUM];
extern ShapeGroup* groups[GROUP_NUM];

// ===== PRIVATE =====

int nextShpIndex() {
	if (freeShpIndex < SHAPE_NUM) {
		freeShpIndex++;
		return freeShpIndex - 1;
	} else {
		return -1; // no more space for more shapes
	}
}

int nextGrpIndex() {
	if (freeGrpIndex < GROUP_NUM) {
		freeGrpIndex++;
		return freeGrpIndex - 1;
	} else {
		return -1; // no more space for more shapes
	}
}


/// ===== PUBLIC =====

// Shape editing. I is the shape ID

void setShapeClr(int i, vec3  value) { shapes[i]->clr = value; }
void setShapePos(int i, vec3  value) { shapes[i]->pos = value; }
void setShapeRv (int i, float value) { shapes[i]->rv  = value; }

// Shape creating. Returns ID of the shape. If ID == -1 then you run out of space for shapes.

int createSphere(vec3 pos, vec3 clr, float rv, float r) {
	int i = nextShpIndex();
	if (i >= 0) { shapes[i] = sph(pos, clr, r, rv); }
	return i;
}

int createCube(vec3 pos, vec3 clr, float rv, vec3 scale) {
	int i = nextShpIndex();
	if (i >= 0) { shapes[i] = cube(pos, clr, scale, 0.0f, rv); }
	return i;
}

int createTorus(vec3 pos, vec3 clr, float rv, float r, float fatness) {
	int i = nextShpIndex();
	if (i >= 0) { shapes[i] = tor(pos, clr, r, fatness, rv); }
	return i;
}

int createCapsule(vec3 start, vec3 clr, float rv, vec3 end, float r) {
	int i = nextShpIndex();
	if (i >= 0) { shapes[i] = caps(start, end, clr, r, rv); }
	return i;
}

int createCylinder(vec3 start, vec3 clr, float rv, vec3 end, float r) {
	int i = nextShpIndex();
	if (i >= 0 ) { shapes[i] = cyl(start, end, clr, r, rv); }
	return i;
}

int createCCone(vec3 start, vec3 clr, float rv, vec3 end, float startR, float endR) {
	int i = nextShpIndex();
	if (i >= 0 ) { shapes[i] = ccone(start, end, clr, startR, endR, rv); }
	return i;
}
