// vim: filetype=c
#ifndef SCENEAPI_H
#define SCENEAPI_H

#include "../vector.h"
#include "shapes.h"

// === private ===
int nextScnIndex(void);
int nextShpIndex(void);
int nextGrpIndex(void);
ShapeType getShapeType(int i);

// === public ===

// shape editing
void setShapeClr(int i, vec3 value);
void setShapePos(int i, vec3  value);
void setShapeRv (int i, float value);

void setCam(vec3 pos, vec3 dir, float angle);

// shape creating
int createSphere  (vec3 pos,   vec3 clr, float rv, float r);
int createCube    (vec3 pos,   vec3 clr, float rv, vec3 scale);
int createTorus   (vec3 pos,   vec3 clr, float rv, float r, float fatness);
int createCapsule (vec3 start, vec3 clr, float rv, vec3 end, float r);
int createCylinder(vec3 start, vec3 clr, float rv, vec3 end, float r);
int createCCone   (vec3 start, vec3 clr, float rv, vec3 end, float startR, float endR);

vec3* getCamPos(void);
vec3* getCamDir(void);

// group creating
int createGroup(int a, int b, OperationType op, float k);

// other
void moveLight(vec3 pos);
float getTime(void);

int compileScene(int scene);
int createScene(int maxShapeNum, int maxGroupNum, int stepsNum, int bounces, float maxTraceDist, float sunSize);
void bindScene (int i);
void bindShader(int i);

#endif
