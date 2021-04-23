// vim: filetype=c
#ifndef SHAPES_H
#define SHAPES_H

#include "../vector.h"

typedef enum ShapeType {
	GROUP    = 0,
	CUBE     = 1,
	SPHERE   = 2,
	CYLINDER = 3,
	BOXFRAME = 4,
	TORUS    = 5,
	CTORUS   = 6,
	LINK     = 7,
	PLANE    = 8,
	HEXPRISM = 9,
	TRIPRISM = 10,
	CAPSULE  = 11,
	CCONE    = 12,
	PYRAMID  = 13,
	TRIANGLE = 14,
	QUAD     = 15
} ShapeType;

typedef enum {
	    CUBE_MASK = 1,
	  SPHERE_MASK = 2,
	CYLINDER_MASK = 4,
	BOXFRAME_MASK = 8,
	   TORUS_MASK = 16,
	  CTORUS_MASK = 32,
  	    LINK_MASK = 64,
	   PLANE_MASK = 128,
	HEXPRISM_MASK = 256,
	TRIPRISM_MASK = 512,
	 CAPSULE_MASK = 1024,
	   CCONE_MASK = 2048,
	 PYRAMID_MASK = 4096,
	TRIANGLE_MASK = 8192,
	    QUAD_MASK = 16384
	//empty slot  = 32768
} ShapeMask;

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

ShapeGroup* group(int a, int b, OperationType op, float k);
Shape* shape(ShapeType type, void* shape, vec3 pos, vec3 clr, float rv);
#endif
