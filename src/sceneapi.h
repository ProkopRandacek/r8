// vim: filetype=c
#include "vector.h"

// private
int nextShpIndex(void);
int nextGrpIndex(void);
ShapeType getShapeType(int i);

// public
void setShapeClr(int i, vec3 value);
void setShapePos(int i, vec3  value);
void setShapeRv (int i, float value);

int createSphere  (vec3 pos,   vec3 clr, float rv, float r);
int createCube    (vec3 pos,   vec3 clr, float rv, vec3 scale);
int createTorus   (vec3 pos,   vec3 clr, float rv, float r, float fatness);
int createCapsule (vec3 start, vec3 clr, float rv, vec3 end, float r);
int createCylinder(vec3 start, vec3 clr, float rv, vec3 end, float r);
int createCCone   (vec3 start, vec3 clr, float rv, vec3 end, float startR, float endR);

int createGroup(int a, int b, OperationType op, float k);
