#version 330
uniform float time;
uniform vec3 lightPos;
uniform ivec2 resolution;
uniform vec3 cam[5];

const int shapeNum = 4;
const int groupNum = 3;

const int shapeSize = 16;
const int groupSize = 6;

uniform float rawShapes[shapeSize * shapeNum];
uniform float rawGroups[groupNum * groupSize];

int ignore = -1;

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

map d2Groups[groupNum];
