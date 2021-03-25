// vim: filetype=c
typedef struct vec3 {
	float x, y, z;
} vec3;


// Vector constructor
vec3 v3(float x, float y, float z);

// Create vector from a single float
vec3 v3f(float a);

// Print vector to console
void vPrint(vec3 a);

// Vector magnitude
float vMag(vec3 a);

// Distance between 2 vectors
float vDist(vec3 a, vec3 b);

// Dot product of 2 vectors
float vDot(vec3 a, vec3 b);

// Cross product of 2 vectors
vec3 vCross(vec3 a, vec3 b);

// Normalize vector
vec3 vNorm(vec3 v);

// Add 2 vectors
vec3 vAdd(vec3 a, vec3 b);

// Multiply 2 vectors
vec3 vMult(vec3 a, vec3 b);

// Multiply vector and a float
vec3 vMultf(vec3 a, float b);

// Return vector pointing from a to b
vec3 vDir(vec3 a, vec3 b);

// Linear interpolation between a and b.
vec3 vLerp(vec3 a, vec3 b, float t);

// Rotate on a circle, c is center, t is input for cos/sin, k controls whether to use sins or cosines to specific axis.
vec3 vRotate(vec3 c, vec3 sinK, vec3 cosK, float t);
