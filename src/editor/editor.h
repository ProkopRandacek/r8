// vim: ft=c
#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"
#include "scene.h"

/**
 * @brief Simple struct to hold editor information
 */
typedef struct Editor {
	/**
	 * @brief The Scene that is being edited by this editor
	 */
	Scene *scene;
} Editor;

/**
 * @brief Construct a new editor instance
 */
Editor *editor_new(Scene *s) LIKE_MALLOC;

/**
 * @brief Draws gui components of this editor
 *
 * Called each frame
 */
void editor_draw(Editor *e);

void editor_draw_scene_tree(Scene *s);
void editor_iterate_scene(Shape *pos, int depth, int *y);
void editor_draw_tree_node(int x, int y, Shape *s);

#endif
