// vim: ft=c
#ifndef COMMON_H
#define COMMON_H

#include <raymath.h>

typedef enum {
	gtUNION,
	gtDIFF,
	gtINTERS,
	gtBLEND,
	gtAVERAGE,
	gtAPPROXIMATE
} GroupType;

typedef enum {
	stPRIMITIVE,
	stGROUP,
	stWRAPPER
} ShapeType;

typedef enum {
	ptSPHERE,
	ptCUBE,
	ptTORUS,
	ptCTORUS,
	ptCYL,
	ptCCONE
} PrimitiveType;

typedef struct {
	double d[12];
	PrimitiveType type;
} Primitive;

typedef struct {
	void* shape;
	char* glslMod; // TODO document this
	ShapeType type;
} Wrapper;

typedef struct {
	void* a;
	void* b;
	ShapeType aType, bType;
	GroupType type;
	double k; // group modificator (for approximations, blend and average)
} Group;

struct Portal;

typedef struct {
	Vector3 pos;  // center
	Vector3 dir;  // forward unit vector
	Vector3 up;   // up unit vector
	Vector2 dims; // scale
	struct Portal* link; // the other portal
} Portal;

typedef struct {
	double eps; // epsilon, for collision detection
	double max_dist; // max render distance
	int rm_iters; // ray march iterations
	int main_iters; // ray bounces / teleports limit

	int portal_num; // number of portals
} Scene;

#endif

