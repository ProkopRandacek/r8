// vim: filetype=c
#include "common.h"

Shape* sph  (vec3 pos,              vec3 clr, float radius, float rv);
Shape* cube (vec3 pos,              vec3 clr, vec3 scale, float roundEdge, float rv);
Shape* tor  (vec3 pos,              vec3 clr, float innerR, float outerR, float rv);
Shape* caps (vec3 start, vec3 end , vec3 clr, float r, float rv);
Shape* cyl  (vec3 start, vec3 stop, vec3 clr, float r, float rv);
Shape* ccone(vec3 start, vec3 end , vec3 clr, float startR, float endR, float rv);

ShapeGroup* group(int a, int b, OperationType op, float k);
Shape* shape(ShapeType type, void* shape, vec3 pos, vec3 clr, float rv);

