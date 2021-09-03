// vim: ft=c
#ifndef SCENE_H
#define SCENE_H

#include <raylib.h>
#include <stdbool.h>

#include "common.h"
#include "shapes.h"

/**
 * Contains all scene settings (There is no global scene setting),
 * Shape hiearchy, Portals and a single Camera
 */
typedef struct Scene {
	// Settings
	float eps;      //!< epsilon, for collision detection
	float max_dist; //!< max render distance
	int rm_iters;   //!< ray march iterations
	int main_iters; //!< ray bounces / teleports limit

	// Shape hierarchy
	Shape *root; //!< root shape of this scene

	Portal **portals; //!< array of Portal pointers

	// Camera
	Camera3D cam;

	// Internal shape stuff
	bool tree_changed;         //!< internal - did the tree hiearchy change since last frame?
	bool primt_changed;        //!< internal - was any shape changed since the last frame?
	bool group_changed;        //!< internal - was any group changed since the last frame?
	bool portal_changed;       //!< internal - was any portal changed since the last frame?
	Shape **flat_prims;        //!< internal - cached array of primitives in the tree
	Shape **flat_groups;       //!< internal - cached array of groups in the tree
	unsigned int primt_count;  //!< internal - number of primitives
	unsigned int group_count;  //!< internal - number of groups
	unsigned int portal_count; //!< internal - number of portals

	// Internal shader stuff
	Shader shader;   //!< internal - the shader (if compiled)
	int res_loc;     //!< internal - the location of resolution vector in shader
	int ro_loc;      //!< internal - the location of Camera position vector in shader
	int ta_loc;      //!< internal - the location of Camera target vector in shader
	int prims_loc;   //!< internal - the location of the primitives float array in shader
	int groups_loc;  //!< internal - the location of the groups float array in shader
	int portals_loc; //!< internal - the location of the groups float array in shader
	//int timeLoc;   //!< internal - the location of the time float in shader
} Scene;

Scene *scene_new(void) LIKE_MALLOC; //!< constructor

void scene_compile(Scene* s); //!< compile shader
void scene_destroy(Scene* s); //!< destructor

void scene_print     (Scene* s); //!< print the scene to stdout
void scene_tick      (Scene* s); //!< sends data to gpu (called each tick)

char* scene_create_sdf(Scene *s); //!< internal

/**
 * @brief gets called whenever the inner shape tree is modified
 *
 * Recalculates some internal variables
 */
void scene_on_tree_update(Scene *s);

#endif
