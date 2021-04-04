#include "serialization.h"
#include "sceneapi.h"
#include "../settings.h"
#include "../debug.h"
#include "../opengl.h"

#include "../settings.h"

int freeShpIndex = 0;
int freeGrpIndex = 0;

extern Shape*      shapes[MAX_SHAPE_NUM];
extern ShapeGroup* groups[MAX_GROUP_NUM];
extern GL* gl;
extern int shapeNum, groupNum, sceneChanged;
extern Camera* cam;

// ===== PRIVATE =====

int nextShpIndex() {
	if (freeShpIndex < MAX_SHAPE_NUM) {
		freeShpIndex++;
		shapeNum = freeShpIndex;
		return freeShpIndex - 1;
	} else {
		return -1; // no more space for more shapes
	}
}

int nextGrpIndex() {
	if (freeGrpIndex < MAX_GROUP_NUM) {
		freeGrpIndex++;
		groupNum = freeGrpIndex;
		return freeGrpIndex - 1;
	} else {
		return -1; // no more space for more shapes
	}
}

inline ShapeType getShapeType(int i) { return shapes[i]->type; }

/// ===== PUBLIC =====

// Shape editing. `i` is the shape ID

void setShapeClr(int i, vec3  value) { shapes[i]->clr = value; sceneChanged = 1; }
void setShapePos(int i, vec3  value) { shapes[i]->pos = value; sceneChanged = 1; }
void setShapeRv (int i, float value) { shapes[i]->rv  = value; sceneChanged = 1; }

// Shape creating. Returns ID of the shape. If ID == -1 then you run out of space for shapes.

int createSphere(vec3 pos, vec3 clr, float rv, float r) {
	int i = nextShpIndex();
	if (i >= 0) { shapes[i] = sph(pos, clr, r, rv); sceneChanged = 1; }
	return i;
}

int createCube(vec3 pos, vec3 clr, float rv, vec3 scale) {
	int i = nextShpIndex();
	if (i >= 0) { shapes[i] = cube(pos, clr, scale, 0.0f, rv); sceneChanged = 1; }
	return i;
}

int createTorus(vec3 pos, vec3 clr, float rv, float r, float fatness) {
	int i = nextShpIndex();
	if (i >= 0) { shapes[i] = tor(pos, clr, r, fatness, rv); sceneChanged = 1; }
	return i;
}

int createCapsule(vec3 start, vec3 clr, float rv, vec3 end, float r) {
	int i = nextShpIndex();
	if (i >= 0) { shapes[i] = caps(start, end, clr, r, rv); sceneChanged = 1; }
	return i;
}

int createCylinder(vec3 start, vec3 clr, float rv, vec3 end, float r) {
	int i = nextShpIndex();
	if (i >= 0 ) { shapes[i] = cyl(start, end, clr, r, rv); sceneChanged = 1; }
	return i;
}

int createCCone(vec3 start, vec3 clr, float rv, vec3 end, float startR, float endR) {
	int i = nextShpIndex();
	if (i >= 0 ) { shapes[i] = ccone(start, end, clr, startR, endR, rv); sceneChanged = 1; }
	return i;
}

// Group creating
int createGroup(int a, int b, OperationType op, float k) {
	int i = nextGrpIndex();
	if (i == -1) { return -1; }
	else {
		groups[i] = group(a, b, op, k);
		sceneChanged = 1;

		return i + MAX_SHAPE_NUM; // external group IDs start on MAX_SHAPE_NUM -> if ID is bigger than MAX_SHAPE_NUM is a group, otherwise its a shape
	}
}

void moveLight(vec3 pos) {
	float f[] = {pos.x, pos.y, pos.z};
	shdSetVec3Array(gl->s, "lightPos", 1, f);
}

float getTime() {
	return (float)glfwGetTime();
}
