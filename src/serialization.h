// vim: filetype=c
#include "shapes.h"

void spheres2floats(float* f, int num, Sphere* spheres);
void cubes2floats(float* f, int num, Cube* cubes);
void cyl2floats(float* f, Cylinder* cyl);

void shapes2floats(float *f, int num, Primitive**  shapes);
void groups2floats(float* f, int num, ShapeGroup** groups);
