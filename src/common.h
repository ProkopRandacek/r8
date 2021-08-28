// vim: ft=c
// This file should be included from every other source in R8
#ifndef COMMON_H
#define COMMON_H

#include <ucw/lib.h>
#include <raymath.h>

// use only the x versions from libucw!
#pragma GCC poison malloc calloc realloc free

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

