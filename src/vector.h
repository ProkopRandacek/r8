// vim: filetype=c
#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <math.h>

typedef struct vec3 {
	float x, y, z;
} vec3;

static vec3 v3     (float x, float y, float z);
static vec3 v3f    (float a);
static void vPrint (vec3 a);
static float vMag  (vec3 a);
static float vDot  (vec3 a, vec3 b);
static float vDist (vec3 a, vec3 b);
static vec3 vNorm  (vec3 v);
static vec3 vAdd   (vec3 a, vec3 b);
static vec3 vMult  (vec3 a, vec3 b);
static vec3 vMultf (vec3 a, float b);
static vec3 vDir   (vec3 a, vec3 b);
static vec3 vLerp  (vec3 a, vec3 b, float t);
static vec3 vCross (vec3 a, vec3 b);


inline vec3 v3(float x, float y, float z) {
	vec3 v;
	v.x = x; v.y = y; v.z = z;
	return v;
}

static inline vec3  v3f    (float a)                 { return v3(a, a, a); }
static inline void  vPrint (vec3 v)                  { printf("%.2f %.2f %.2f", v.x, v.y, v.z); }
static inline float vMag   (vec3 a)                  { return sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z)); }
static inline float vDot   (vec3 a, vec3 b)          { return a.x * b.x + a.y * b.y; }
static inline float vDist  (vec3 a, vec3 b)          { return vMag(vDir(a, b)); }
static inline vec3  vNorm  (vec3 a)                  { return vMultf(a, 1 / vMag(a)); }
static inline vec3  vAdd   (vec3 a, vec3 b)          { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
static inline vec3  vMult  (vec3 a, vec3 b)          { return v3(a.x * b.x, a.y * b.y, a.z * b.z); }
static inline vec3  vMultf (vec3 a,   float b)       { return v3(a.x * b, a.y * b, a.z * b); }
static inline vec3  vDir   (vec3 a, vec3 b)          { return vAdd(b, vMultf(a, -1.0f)); }
static inline vec3  vLerp  (vec3 a, vec3 b, float c) { return vAdd(a, vMultf(vDir(a, b), c)); }

inline vec3 vCross(vec3 a, vec3 b) {
	vec3 c;
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	return c;
}

#endif
