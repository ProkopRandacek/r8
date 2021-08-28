// vim: ft=c
#ifndef SPHERE_H
#define SPHERE_H

#include "shapes.h"

/** @brief Sphere Shape constructor */
Shape* sphere_new(Vector3 pos, Vector4 clr, float r) LIKE_MALLOC;

#endif

