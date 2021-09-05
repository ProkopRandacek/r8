// vim: ft=c
#ifndef SHAPE_H
#define SHAPE_H

#include <stdbool.h>
#include "common.h"

#define SHAPE_NAME_LEN 32

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

	// Editor properties
	bool collapsed; //!< Is this wrapper collapsed in the editor?
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

	// Editor properties
	bool collapsed; //!< Is this group collapsed in the editor?
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
		Group     g; //!< The Group inside this Shape (if type is stGROUP).
		Wrapper   w; //!< The Wrapper inside this Shape (if type is stWRAPPER).
		Primitive p; //!< The Primitive inside this Shape (if type is stPRIMITIVE).
	};
	ShapeType type; //!< The type of this shape.

	// Editor properties
	char name[SHAPE_NAME_LEN]; //!< The name that is shown in the editor.
} Shape;

#include "group.h"
#include "portal.h"

/** @brief Sphere Shape constructor */
Shape* sphere_new(const char* name, Vector3 pos, Vector4 clr, float r) LIKE_MALLOC;

/** @brief Cube Shape constructor */
Shape* cube_new(const char* name, Vector3 pos, Vector4 clr, Vector3 scl) LIKE_MALLOC;

Vector3 prim_get_pos(Primitive *p);
Vector4 prim_get_clr(Primitive *p);
float prim_get_rad1(Primitive *p);
float prim_get_rad2(Primitive *p);

#endif
