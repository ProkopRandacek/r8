#version 330

// when editing these you have to edit it in settings.h too!!!!!
#define maxShapeNum 10
#define maxGroupNum 10

#define shapeSize 16
#define groupSize 4

uniform int groupNum, shapeNum, rawGroups[groupSize * maxGroupNum], shapeTypes[maxShapeNum];
uniform float rawShapes[shapeSize * maxShapeNum];
uniform ivec2 resolution;
uniform vec3 lightPos, cam[5];

vec4  d2GroupsC[maxGroupNum]; // colors
float d2GroupsD[maxGroupNum], d2Shapes[maxShapeNum];

#define ones vec3(1.0)
const vec3 smol = vec3(0.02, 0.0, 0.0);

out vec4 outColor;

struct rayHit {
	vec4 hitPos, surfaceClr;
	// hitPos.w == 0 -> didnt hit else hit
};

struct map {
	vec4 clr;
	float d;
};

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
