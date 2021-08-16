#include "portal.h"

void portal_serialize(float* arr, Portal p) {
	arr[0] = p.c   .x; arr[1]  = p.c   .y; arr[2] = p.c  .z;
	arr[3] = p.dir .x; arr[4]  = p.dir .y; arr[5] = p.dir.z;
	arr[6] = p.up  .x; arr[7]  = p.up  .y; arr[8] = p.up .z;
	arr[8] = p.dims.x; arr[10] = p.dims.y;
}
