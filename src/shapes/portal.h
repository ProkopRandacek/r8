// vim: ft=c
#ifndef PORTAL_H
#define PORTAL_H

#include "../common.h"

Portal* portal_new(Vector3 pos, Vector3 up, Vector3 dir, Vector2 dims) LIKE_MALLOC;

#endif

