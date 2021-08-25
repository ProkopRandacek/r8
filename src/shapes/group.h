// vim: ft=c
#ifndef GROUP_H
#define GROUP_H

#include "../common.h"

/** @brief Group constructor */
Shape *group_new(Shape *a, Shape *b, GroupType gt, float k) LIKE_MALLOC;

#endif
