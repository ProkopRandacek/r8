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
	pSPHERE,
	pCUBE,
	pTORUS,
	pCTORUS,
	pCYL,
	pCCONE
} Primitive;

typedef struct {
	double f[8];
	Primitive type;
} Shape;

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

typedef struct {
	Vector3 c;    // center
	Vector3 dir;  // forward unit vector
	Vector3 up;   // up unit vector
	Vector2 dims; // scale
} Portal;

typedef struct {
	Portal a, b;
} PortalGroup;

typedef struct {
	double eps; // epsilon, for collision detection
	double max_dist; // max render distance
	int rm_iters; // ray march iterations
	int main_iters; // ray bounces / teleports limit

	int portal_num; // number of portals
} Scene;

#endif
