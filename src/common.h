// vim: ft=c
#ifndef COMMON_H
#define COMMON_H

#include <raymath.h>

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
	float k; // group modificator (for approximations, blend and average)
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
	Vector3 pos;  // center
	Vector3 dir;  // forward unit vector
	Vector3 up;   // up unit vector
	Vector2 dims; // scale
	struct Portal* link; // the other portal
} Portal;

typedef struct Scene {
	float eps; // epsilon, for collision detection
	float max_dist; // max render distance
	int rm_iters; // ray march iterations
	int main_iters; // ray bounces / teleports limit

	Shape* root; // root shape of this scene

	Camera cam;
	Shader shader; // the shader if compiled
	int resLoc;
	int roLoc;
	int taLoc;
} Scene;

#endif

