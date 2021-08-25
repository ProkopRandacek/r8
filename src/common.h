// vim: ft=c
// This file should be included from every other source in R8
#ifndef COMMON_H
#define COMMON_H

#include <ucw/lib.h>
#include <raymath.h>

// use only the x versions from libucw
#pragma GCC poison malloc calloc realloc free

#define SHAPE_SIZE 12

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
	gtUNION,      //!< Simply join the two Shapes.
	gtDIFF,       //!< Inverse the second Shape and join the two Shapes.
	gtINTERS,     //!< Resulting Shape exists on the intersection of the two Shapes.
	gtBLEND,      //!< Smoothly join the two Shapes. Smoothnes is set by the k.
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
 * @brief a primitive shape representation
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
 * @brief wraps a single other Shape
 *
 * Wrapping is used to transform distance fields of the sub shape.
 */
typedef struct Wrapper {
	struct Shape *shape; //!< The inner shape
	//char* glslMod; // TODO idk
} Wrapper;

/**
 * @brief group of two shapes
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
 * Portals without a valid linked portal are rendered purple
 */
typedef struct Portal {
	Vector3 pos;  //!< position of the portal
	Vector3 dir;  //!< forward unit vector
	Vector3 up;   //!< up unit vector
	Vector2 dims; //!< portal scale
	struct Portal* link; //!< linked portal
} Portal;

/**
 * @brief Representation of a single Scene.
 *
 * Contains all scene settings (There is no global scene setting),
 * Shape hiearchy, Camera and Portals.
 */
typedef struct Scene {
	// Settings
	float eps;      //!< epsilon, for collision detection
	float max_dist; //!< max render distance
	int rm_iters;   //!< ray march iterations
	int main_iters; //!< ray bounces / teleports limit

	// Shape hiearchy
	Shape* root; //!< root shape of this scene

	// Camera
	Camera cam;

	// Internal shader stuff
	Shader shader; //!< the shader (if compiled)
	int resLoc;    //!< the location of resolution vector in shader
	int roLoc;     //!< the location of Camera position vector in shader
	int taLoc;     //!< the location of Camera target vector in shader
} Scene;

#endif

