#include "sceneapi.h"
#include "scene.h"
#include "../shader/shader.h"
#include "../settings.h"
#include "../debug.h"
#include "../settings.h"

int freeScnIndex = 0;
int sceneIndex = 0;

extern int sceneChanged; // from scene.c
extern Scene* scenes[MAX_SHAPE_NUM]; // from scene.c
extern unsigned int bindedScene; // from scene.c

// ===== PRIVATE =====

int nextScnIndex() {
	if (freeScnIndex < MAX_SCENE_NUM) {
		freeScnIndex++;
		return freeScnIndex - 1;
	} else {
		return -1; // no more space for more shapes
	}
}

int nextShpIndex() {
	if (scenes[bindedScene]->freeShpIndex < MAX_SHAPE_NUM) {
		scenes[bindedScene]->freeShpIndex++;
		scenes[bindedScene]->shapeNum = scenes[bindedScene]->freeShpIndex;
		return scenes[bindedScene]->freeShpIndex - 1;
	} else {
		return -1; // no more space for more shapes
	}
}

int nextGrpIndex() {
	if (scenes[bindedScene]->freeGrpIndex < MAX_GROUP_NUM) {
		scenes[bindedScene]->freeGrpIndex++;
		scenes[bindedScene]->groupNum = scenes[bindedScene]->freeGrpIndex;
		return scenes[bindedScene]->freeGrpIndex - 1;
	} else {
		return -1; // no more space for more shapes
	}
}

inline ShapeType getShapeType(int i) { return scenes[bindedScene]->shapes[i]->type; }

/// ===== PUBLIC =====

void bindScene (int i) { bindedScene  = (unsigned int)i; }
void bindShader(int i) {
	//printf("%d, %d\n", i, (unsigned int)i);
	activateShader((unsigned int)i);
	sendObjects();
}

// Shape editing. `i` is the shape ID
void setShapeClr(int i, vec3  value) { scenes[bindedScene]->shapes[i]->clr = value; sceneChanged = 1; }
void setShapePos(int i, vec3  value) { scenes[bindedScene]->shapes[i]->pos = value; sceneChanged = 1; }
void setShapeRv (int i, float value) { scenes[bindedScene]->shapes[i]->rv  = value; sceneChanged = 1; }

// Shape creating. Returns ID of the shape. If ID == -1 then you run out of space for shapes.
int createSphere(vec3 pos, vec3 clr, float rv, float r) {
	int i = nextShpIndex();
	if (i >= 0) { scenes[bindedScene]->shapes[i] = sph(pos, clr, r, rv); sceneChanged = 1; }
	return i;
}

int createCube(vec3 pos, vec3 clr, float rv, vec3 scale) {
	int i = nextShpIndex();
	if (i >= 0) { scenes[bindedScene]->shapes[i] = cube(pos, clr, scale, 0.0f, rv); sceneChanged = 1; }
	return i;
}

int createTorus(vec3 pos, vec3 clr, float rv, float r, float fatness) {
	int i = nextShpIndex();
	if (i >= 0) { scenes[bindedScene]->shapes[i] = tor(pos, clr, r, fatness, rv); sceneChanged = 1; }
	return i;
}

int createCapsule(vec3 start, vec3 clr, float rv, vec3 end, float r) {
	int i = nextShpIndex();
	if (i >= 0) { scenes[bindedScene]->shapes[i] = caps(start, end, clr, r, rv); sceneChanged = 1; }
	return i;
}

int createCylinder(vec3 start, vec3 clr, float rv, vec3 end, float r) {
	int i = nextShpIndex();
	if (i >= 0 ) { scenes[bindedScene]->shapes[i] = cyl(start, end, clr, r, rv); sceneChanged = 1; }
	return i;
}

int createCCone(vec3 start, vec3 clr, float rv, vec3 end, float startR, float endR) {
	int i = nextShpIndex();
	if (i >= 0 ) { scenes[bindedScene]->shapes[i] = ccone(start, end, clr, startR, endR, rv); sceneChanged = 1; }
	return i;
}

// Group creating
int createGroup(int a, int b, OperationType op, float k) {
	int i = nextGrpIndex();
	if (i == -1) { return -1; }
	else {
		scenes[bindedScene]->groups[i] = group(a, b, op, k);
		sceneChanged = 1;

		return i + MAX_SHAPE_NUM; // external group IDs start on MAX_SHAPE_NUM -> if ID is bigger than MAX_SHAPE_NUM is a group, otherwise its a shape
	}
}

int createScene(int maxShapeNum, int maxGroupNum, int stepsNum, int bounces, float maxTraceDist, float sunSize) {
	int i = nextScnIndex();
	if (i >= 0) scenes[i] = scene(maxShapeNum, maxGroupNum, stepsNum, bounces, maxTraceDist, sunSize);
	return i;
}

int compileScene(int scene) {
	int i = (int)createShader(scenes[scene]);
	return i;
}

// Other
//float getTime() { return (float)glfwGetTime(); }
vec3* getCamPos() { return &scenes[bindedScene]->cam->pos; }
vec3* getCamDir() { return &scenes[bindedScene]->cam->forward; }
