// vim: filetype=c
#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "shapes.h"
#include "camera.h"

void shapes2floats(float* f, unsigned int num, Shape*      shapes[num]);
void groups2floats(int*   f, unsigned int num, ShapeGroup* groups[num]);
void shapes2types (int*   f, unsigned int num, Shape*         shp[num]);

void sphere2floats(float* f, Sphere*   s);
void cube2floats  (float* f, Cube*     s);
void torus2floats (float* f, Torus*    s);
void caps2floats  (float* f, Capsule*  s);
void cyl2floats   (float* f, Cylinder* s);
void ccone2floats (float* f, CCone*    s);

void cam2floats(Camera* cam, float* f);
#endif
