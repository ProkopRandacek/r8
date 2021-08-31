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

#endif
