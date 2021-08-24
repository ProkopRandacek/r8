// vim: ft=c
// This file should be included from every other source in R8
#ifndef COMMON_H
#define COMMON_H

#include <ucw/lib.h>
#include <raymath.h>

// use only the x versions from libucw
#pragma GCC poison malloc calloc realloc free

#define SHAPE_SIZE 12

typedef enum PrimitiveType {
	ptSPHERE,
	ptCUBE,
	ptTORUS,
	ptCTORUS,
	ptCYL,
	ptCCONE
} PrimitiveType;

typedef enum GroupType {
	gtUNION,
	gtDIFF,
	gtINTERS,
	gtBLEND,
	gtAVERAGE,
	gtAPPROXIMATE
} GroupType;

typedef enum ShapeType {
	stPRIMITIVE,
	stGROUP,
	stWRAPPER
} ShapeType;

typedef struct Primitive {
	float d[12];
	PrimitiveType type;
} Primitive;

typedef struct Shape Shape;

typedef struct Wrapper {
	struct Shape *shape;
	char* glslMod; // TODO document this
	ShapeType type;
} Wrapper;

typedef struct Group {
	struct Shape *a, *b;
	GroupType type;
	float k; //!< group modificator (for approximations, blend and average)
} Group;

typedef struct Shape {
	union {
		Group     g;
		Wrapper   w;
		Primitive p;
	};
	ShapeType type;
} Shape;

typedef struct Portal {
	Vector3 pos;  //!< position of the portal
	Vector3 dir;  //!< forward unit vector
	Vector3 up;   //!< up unit vector
	Vector2 dims; //!< portal scale
	struct Portal* link; //!< linked portal
} Portal;

typedef struct Scene {
	float eps; //!< epsilon, for collision detection
	float max_dist; //!< max render distance
	int rm_iters; //!< ray march iterations
	int main_iters; //!< ray bounces / teleports limit

	Shape* root; //!< root shape of this scene

	Camera cam;
	Shader shader; //!< the shader (if compiled)
	int resLoc;
	int roLoc;
	int taLoc;
} Scene;

#endif

