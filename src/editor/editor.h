// vim: ft=c
#ifndef EDITOR_H
#define EDITOR_H

#include <microui.h>
#include "scene.h"
#include "common.h"

/**
 * @brief Simple struct to hold editor information
 */
typedef struct Editor {
	/**
	 * @brief The Scene that is being edited by this editor
	 */
	Scene *scene;

	/**
	 * @brief UI unit constant. Change to resize editor UI elements
	 */
	float unit;

	/**
	 * @brief describes the UI panels sizes
	 *
	 * It is the position of the resize button in the window.
	 * Panels use this to calculate their positions and sizes.
	 */
	Vector2 editor_layout;

	/**
	 * @brief Shape that is currently selected in the editor.
	 */
	Shape* selected_shape;
} Editor;

/**
 * @brief Creates a new Editor instance.
 */
Editor *editor_new(Scene *s) LIKE_MALLOC;

/**
 * @brief Deletes Editor instance.
 */
void editor_destroy(Editor *e);

/**
 * @brief Renders all Editor UI elements.
 */
void editor_draw(mu_Context* ctx, Editor *e);

void editor_properties(mu_Context *ctx, Editor *e);

#endif

