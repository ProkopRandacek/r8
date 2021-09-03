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
		editor_layout.x = MAX(MIN(GetScreenWidth() - UNIT,  (float)GetMouseX() - UNIT / 2), UNIT * 5);
		editor_layout.y = MAX(MIN(GetScreenHeight() - UNIT, (float)GetMouseY() - UNIT / 2), UNIT * 5);
	}
	resize_was_active = resize_active;

	const char version[] = "R8 " R8_VERSION " (" R8_COMMIT_HASH ")";
	Rectangle ver_box = (Rectangle){
		GetScreenWidth() - UNIT * 10.5f,
		GetScreenHeight() - GuiGetStyle(DEFAULT, TEXT_SIZE),
		UNIT * 10,
		GuiGetStyle(DEFAULT, TEXT_SIZE)
	};
	GuiLabel(ver_box, version);

	editor_draw_toolbar();
}

