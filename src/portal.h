#include <raymath.h>

typedef struct {
	Vector3 c;    // center
	Vector3 dir;  // forward unit vector
	Vector3 up;   // up unit vector
	Vector2 dims; // scale
} Portal;

typedef struct {
	Portal a, b;
} PortalGroup;
