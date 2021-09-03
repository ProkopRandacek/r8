// vim: ft=c
#ifndef CUBE_H
#define CUBE_H

#include "shapes.h"

/** @brief Cube Shape constructor */
Shape* cube_new(const char* name, Vector3 pos, Vector4 clr, Vector3 scl) LIKE_MALLOC;

#endif

