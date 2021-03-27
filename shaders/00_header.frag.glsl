#version 330
uniform float time;
uniform vec3 lightPos;
uniform ivec2 resolution;
uniform vec3 cam[5];
uniform int groupNum; // the number of created groups

const int maxShapeNum = 4;
const int maxGroupNum = 3;

const int shapeSize = 16;
const int groupSize = 6;

int ignore = -1;

uniform float rawShapes[shapeSize * maxShapeNum];
uniform float rawGroups[groupSize * maxGroupNum];

out vec4 outColor;

struct rayHit {
	vec3 hitPos;
	vec4 surfaceClr;
	int steps;
	bool hit;
	float shadow;
};

struct map {
	vec4 clr;
	float d;
};

map d2Groups[maxGroupNum];
