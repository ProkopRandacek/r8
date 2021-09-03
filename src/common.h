// vim: ft=c
// This file should be included from every other source in R8
#ifndef COMMON_H
#define COMMON_H

#include <raymath.h>

#define NONRET              __attribute__((noreturn))
#define CONST               __attribute__((const))
#define FORMAT_CHECK(x,y,z) __attribute__((format(x,y,z)))
#define LIKE_MALLOC         __attribute__((malloc))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/** @brief number of floats needed to represent any Primitive */
#define PRIMT_SIZE 12

/** @brief number of floats needed to represent a Portal */
#define PORTAL_SIZE 12

/**
 * @brief maximum number of Portals that can exist in a single Scene
 *
 * Because I don't feel like making something dynamic right now...
 * TODO
 * */
#define MAX_PORTALS_PER_SCENE 64



#endif

