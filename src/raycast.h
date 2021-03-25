#include "vector.h"

typedef struct rayHit {
	vec3 hitPos;
	char hit; //bool
	unsigned int steps;
} rayHit;
