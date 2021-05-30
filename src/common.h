// vim: filetype=c
#ifndef COMMON_H
#define COMMON_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "settings.h"

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

typedef struct vec3 {
	float x, y, z;
} vec3;

typedef struct GL {
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int s;
	GLFWwindow* window;
} GL;

typedef struct Shape { // General shape
	vec3 pos; // in some cases only start of the shape
	vec3 clr; // color
	float rv; // refletiveness

	void* shape; // rest of the shape
	ShapeType type; // type of the shape
} Shape;

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

typedef struct ShapeGroup {
	OperationType op;
	float k; // modifier for operation.
	int a; // id of the shape A
	int b; // id of the shape B
} ShapeGroup;

typedef struct Camera {
	vec3 pos;
	vec3 tl; // The direction of most top left ray
	vec3 tr; // top right
	vec3 bl; // ...
	vec3 br; // ...

	vec3 left; // these are for C, not actually send to the GPU
	vec3 forward;
} Camera;

typedef struct Scene {
	ShapeMask shapeMask;
	Camera* cam;
	Shape      **shapes; // array of pointers to shape
	ShapeGroup **groups; // array of pointers to group
	int maxShapeNum;
	int maxGroupNum;
	int shapeNum;
	int groupNum;
	int stepsNum;
	int bounces;
	int freeGrpIndex;
	int freeShpIndex;
	float epsilon;
	float collisionThreshold;
	float shadowCollisionThreshold;
	float backStepK;
	float maxTraceDist;
	float sunSize;
} Scene;

#endif

