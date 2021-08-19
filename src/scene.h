// vim: ft=c
#ifndef SCENE_H
#define SCENE_H

#include "common.h"

Scene *scene_new(); // constructor

void scene_compile(Scene* s); // compile shader
void scene_update (Scene* s); // sends data to gpu (called each tick)
void scene_destroy(Scene* s); // destructor

#endif
