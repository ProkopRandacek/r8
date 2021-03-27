// vim: filetype=c
#include "vector.h"
#include "shapes.h"

// === private ===
int nextShpIndex(void);
int nextGrpIndex(void);
ShapeType getShapeType(int i);

// === public ===

// shape editing
void setShapeClr(int i, vec3 value);
void setShapePos(int i, vec3  value);
void setShapeRv (int i, float value);

// shape creating
int createSphere  (vec3 pos,   vec3 clr, float rv, float r);
int createCube    (vec3 pos,   vec3 clr, float rv, vec3 scale);
int createTorus   (vec3 pos,   vec3 clr, float rv, float r, float fatness);
int createCapsule (vec3 start, vec3 clr, float rv, vec3 end, float r);
int createCylinder(vec3 start, vec3 clr, float rv, vec3 end, float r);
int createCCone   (vec3 start, vec3 clr, float rv, vec3 end, float startR, float endR);

// group creating
int createGroup(int a, int b, OperationType op, float k);

// other
void moveLight(vec3 pos);
float getTime(void);
