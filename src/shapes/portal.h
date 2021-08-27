// vim: ft=c
#ifndef PORTAL_H
#define PORTAL_H

#include <ucw/slists.h>
#include "../common.h"

/**
 * @brief A rectangle that teleports rays to it's linked rectangle
 *
 * Portals are rendered separately from other shapes.
 * They have no hierarchy like Shapes e.g. you can't
 * create a Group of two portals and blend them together.
 *
 * Portals without a valid linked portal are not rendered at all.
 *
 * If you link two portals of different scales, the shapes visible
 * through the portal bridge will appear scaled in the same way.
 */
typedef struct Portal {
	/**
	 * @brief position of the portal.
	 * more precisely the center of it.
	 */
	Vector3 pos;
	Vector3 dir; //!< forward unit vector
	Vector3 up;  //!< up unit vector
	Vector2 scl; //!< portal scale
	struct Portal* link; //!< linked portal
} Portal;

/**
 * @brief Portal constructor
 *
 * @param pos The center
 * @param up Up unit vector
 * @param dir Foward unit vector
 * @param scl Scale
 * */
Portal* portal_new(Vector3 pos, Vector3 dir, Vector3 up, Vector2 scl) LIKE_MALLOC;

/**
 * @brief Writes an array of Portals into a float array
 *
 * Is used to serialize all portals in a scene and send them over to the shader.
 * */
float* portal_write_bulk(Portal **p, int count);

#endif

