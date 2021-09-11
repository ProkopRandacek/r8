#include "_raygui.h"
#include <raylib.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

bool resize_was_active = false;

Editor *editor_new(Scene *s) {
	Editor *e = xmalloc_zero(sizeof(Editor));
	e->scene = s;
	e->unit = 20.0f;

	e->editor_layout = (Vector2){ GetScreenWidth() / 4.0f, GetScreenHeight() / 2.0f };

	return e;
}

void editor_draw(Editor *e) {
	editor_draw_scene_tree(e);
	editor_draw_properties(e);

	// Resize button
	Rectangle resize = (Rectangle){ e->editor_layout.x, e->editor_layout.y, e->unit, e->unit };
	bool resize_active = GuiToggle(resize, resize_was_active ? "#67#" : "#51#", resize_was_active);
	if (resize_active) {
		float w = GetScreenWidth();
		float h = GetScreenHeight();
		e->editor_layout.x = MAX(MIN(w - e->unit, (float)GetMouseX() - e->unit / 2), e->unit * 6.5f);
		e->editor_layout.y = MAX(MIN(h - e->unit, (float)GetMouseY() - e->unit / 2), e->unit * 0);
	}
	resize_was_active = resize_active;

	editor_draw_toolbar(e);
}

void editor_destroy(Editor *e) {
	xfree(e);
}

