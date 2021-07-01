// vim: filetype=c
#include "common.h"

typedef struct Vector {
	double x, y, z;
} Vector;

static vec3 v3     (float x, float y, float z);
static vec3 v3f    (float a);
static void vPrint (vec3 a);
static float vMag  (vec3 a);
static float vDot  (vec3 a,  vec3 b);
static float vDist (vec3 a,  vec3 b);
static vec3 vNorm  (vec3 v);
static vec3 vAdd   (vec3 a,  vec3 b);
static vec3 vMult  (vec3 a,  vec3 b);
static vec3 vMultf (vec3 a, float b);
static vec3 vDir   (vec3 a,  vec3 b);
static vec3 vLerp  (vec3 a,  vec3 b, float t);
static vec3 vCross (vec3 a,  vec3 b);

