#version 330
//uniform float time;
uniform vec3 lightPos, cam[5];
uniform ivec2 resolution;
const int groupNum = 3;
const int shapeNum = 4;

#define maxShapeNum 4
#define maxGroupNum 3

#define shapeSize 16
#define groupSize 4

#define ones vec3(1.0)
vec2 smol = vec2(0.02, 0);

const float rawShapes[shapeSize * maxShapeNum] = float[](
		0.50, 2.10, 0.50, 0.70, 0.00, 0.70, 0.00, 0.00, 0.00, 0.30, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
		0.50, 1.90, 0.50, 0.70, 0.00, 0.70, 0.00, 0.00, 0.00, 0.25, 0.09, 0.00, 0.00, 0.00, 0.00, 0.00,
		0.50, 2.20, 0.50, 0.70, 0.00, 0.70, 0.50, 1.30, 0.50, 0.17, 0.30, 0.00, 0.00, 0.00, 0.00, 0.00,
		0.50, 1.30, 0.50, 0.70, 0.00, 0.70, 0.50, 1.00, 0.50, 0.40, 0.40, 0.00, 0.00, 0.00, 0.00, 0.00
		);
const int rawGroups[groupSize * maxGroupNum] = int[]( 0, 1, 0, 0, 2, 3, 1, 200, 4, 5, 1, 19);
const int shapeTypes[maxShapeNum] = int[]( 2, 5, 12, 12);

out vec4 outColor;

struct rayHit {
	vec4 hitPos, surfaceClr;
	// hitPos.w == 0 -> didnt hit else hit
};

struct map {
	vec4 clr;
	float d;
};

vec4  d2GroupsC[maxGroupNum]; // colors
float d2GroupsD[maxGroupNum]; // distances
float d2Shapes[maxShapeNum];  // distances to shapes

// Functions to find information in groups array
#define gF(i, o) rawGroups[groupSize * i + o] // returns float on position `o` from group on index `i`

#define gA(i)  gF(i, 0)                 // A shape/group index
#define gB(i)  gF(i, 1)                 // B shape/group index
#define gOp(i) gF(i, 2)                 // group operation
#define gK(i)  float(gF(i, 3)) / 1000.0 // group operation modifier

// Functions to find information in shapes array
#define sF(i, o) rawShapes[shapeSize * i + o]               // return float on position `o` from shape on index `i`
#define sV(i, o) vec3(sF(i, o), sF(i, o + 1), sF(i, o + 2)) //vector from 3 floats on position from o to o + 2 from shape on index `i`

#define sT(i)  shapeTypes[i]             // type
#define sR(i)  sF(i, 9)                  // radius
#define sRs(i) sF(i, 15)                 // refletiveness
#define sP(i)  sV(i, 0)                  // position
#define sS(i)  sV(i, 6)                  // scale
#define sC(i)  vec4(sV(i, 3), sRs(i))    // color
#define sRV(i) vec2(sF(i, 9), sF(i, 10)) // radius vector
