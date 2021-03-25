// vim: filetype=c

#include "vector.h"

typedef enum ShapeType {
	GROUP, CUBE, SPHERE, CYLINDER, BOXFRAME, TORUS, CTORUS, LINK, PLANE, HEXPRISM, TRIPRISM, CAPSULE, CCONE, PYRAMID, TRIANGLE, QUAD
} ShapeType;

typedef enum OperationType {
	NORMAL, BLEND, CUT, MASK, AVERAGE
} OperationType;

// ==== start of shapes ====

typedef struct Spheres {
	float radius;
} Sphere;

typedef struct Cube {
	vec3 scale;
	float roundEdge;
} Cube;

typedef struct Torus {
	float radius;
	float fatness;
} Torus;

typedef struct Capsule { // Like cylinder but round
	vec3 end;
	float radius;
} Capsule;

typedef struct Cylinder {
	vec3 end;
	float radius;
} Cylinder;

typedef struct CCone {
	vec3 end;
	float startR;
	float endR;
} CCone;

// ==== end of shapes ====

typedef struct Shape { // General shape
	vec3 pos; // in some cases only start of the shape
	vec3 clr; // color
	float rv; // refletiveness

	void* shape; // rest of the shape
	ShapeType type; // type of the shape
} Shape;

typedef struct ShapeGroup {
	ShapeType ta;
	ShapeType tb;
	OperationType op;
	float k; // modifier for operation.
	int a; // id of the shape A
	int b; // id of the shape B
} ShapeGroup;

Shape* sph  (vec3 pos,              vec3 clr, float radius, float rv);
Shape* cube (vec3 pos,              vec3 clr, vec3 scale, float roundEdge, float rv);
Shape* tor  (vec3 pos,              vec3 clr, float innerR, float outerR, float rv);
Shape* caps (vec3 start, vec3 end,  vec3 clr, float r, float rv);
Shape* cyl  (vec3 start, vec3 stop, vec3 clr, float r, float rv);
Shape* ccone(vec3 start, vec3 end,  vec3 clr, float startR, float endR, float rv);

ShapeGroup* group(ShapeType ta, int a, ShapeType tb, int b, OperationType op, float k);
Shape* shape(ShapeType type, void* shape, vec3 pos, vec3 clr, float rv);
