// vim: ft=c
#ifndef GROUP_H
#define GROUP_H

#include "shapes.h"

/** @brief Group constructor */
Shape *group_new(char* name, Shape *a, Shape *b, GroupType gt, float k) LIKE_MALLOC;

#endif
