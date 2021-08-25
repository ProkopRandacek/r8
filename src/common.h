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

/** @brief type of a Primitive */
typedef enum PrimitiveType {
	ptSPHERE,
	ptCUBE,
	ptTORUS,
	ptCTORUS,
	ptCYL,
	ptCCONE
} PrimitiveType;

/** @brief type of a Group */
typedef enum GroupType {
	gtUNION,      //!< Resulting Shape exists on the union of the two sub-shapes.
	gtDIFF,       //!< Resulting Shape exists on the union of the first shape and negated second shape.
	gtINTERS,     //!< Resulting Shape exists on the intersection of the two sub-shapes.
	gtBLEND,      //!< Smooth gtUNION. Smoothness set by Group's k value.
	gtAVERAGE,    //!< Average out the two distance fields.
	gtAPPROXIMATE //!< Only calculates the second distance field if the first one resulted in a value less than k.
} GroupType;

/** @brief type of a Shape */
typedef enum ShapeType {
	stPRIMITIVE, //!< The shape contains a Primitive
	stGROUP,     //!< The shape contains a Group
	stWRAPPER    //!< The shape contains a Wrapper
} ShapeType;

/**
 * @brief signle simple geometric shape.
 *
 * R8 can render these natively.
 * @see PrimitiveType
 */
typedef struct Primitive {
	float d[12];        //!< see design.md
	PrimitiveType type; //!< The type of this shape
} Primitive;

typedef struct Shape Shape;

/**
 * @brief wraps a single other Shape.
 *
 * Wrapping is used to transform distance fields of the sub shape.
 */
typedef struct Wrapper {
	struct Shape *shape; //!< The inner shape
	//char* glslMod; // TODO idk
} Wrapper;

/**
 * @brief group of two shapes.
 *
 * Combines two shapes of any kind. The result depends on type of the group.
 * @see GroupType
 */
typedef struct Group {
	struct Shape *a, *b; //!< The two inner shapes
	GroupType type;      //!< The type of this group
	float k;             //!< group modificator (for approximations, blend and average)
} Group;

/**
 * @brief single or a tree of objects in a Scene.
 *
 * A general shape in the scene.
 *
 * Can be either a Group, a Wrapper or a Primitive.
 * @see ShapeType
 */
typedef struct Shape {
	union {
		Group     g; //!< The Group inside this Shape (if type is stGROUP)
		Wrapper   w; //!< The Wrapper inside this Shape (if type is stWRAPPER)
		Primitive p; //!< The Primitive inside this Shape (if type is stPRIMITIVE)
	};
	ShapeType type; //!< The type of this shape.
} Shape;

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
	Vector3 dir;  //!< forward unit vector
	Vector3 up;   //!< up unit vector
	Vector2 dims; //!< portal scale
	struct Portal* link; //!< linked portal
} Portal;

#endif

