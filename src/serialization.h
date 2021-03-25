// vim: filetype=c
#include "shapes.h"

void shapes2floats(float *f, unsigned int num, Shape*      shapes[num]);
void groups2floats(float* f, unsigned int num, ShapeGroup* groups[num]);

void sphere2floats(float* f, Sphere* s);
void cube2floats(float* f, Cube* s);
void torus2floats(float* f, Torus* s);
void caps2floats(float* f, Capsule* s);
void cyl2floats(float* f, Cylinder* s);
void ccone2floats(float* f, CCone*s);
