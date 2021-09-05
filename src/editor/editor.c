#include "_raygui.h"
#include <raylib.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

bool resize_was_active = false;
Vector2 editor_layout;

Editor *editor_new(Scene *s) {
	Editor *e = xmalloc(sizeof(Editor));
	e->scene = s;

	editor_layout = (Vector2){ GetScreenWidth() / 4.0f, GetScreenHeight() / 2.0f };

	return e;
}

void editor_draw(Editor *e) {
	editor_draw_scene_tree(e->scene);
	editor_draw_properties();

	// Resize button
	Rectangle resize = (Rectangle){ editor_layout.x, editor_layout.y, UNIT, UNIT };
	bool resize_active = GuiToggle(resize, resize_was_active ? "#67#" : "#51#", resize_was_active);
	if (resize_active) {
		float w = GetScreenWidth();
		float h = GetScreenHeight();
		editor_layout.x = MAX(MIN(w - UNIT, (float)GetMouseX() - UNIT / 2), UNIT * 6.5f);
		editor_layout.y = MAX(MIN(h - UNIT, (float)GetMouseY() - UNIT / 2), UNIT * 0);
	}
	resize_was_active = resize_active;

	editor_draw_toolbar();
}

void editor_destroy(Editor *e) {
	xfree(e);
}

