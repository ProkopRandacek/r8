#include "_raygui.h"
#include <raylib.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

void editor_draw_scene_tree(Scene *s) {
	Rectangle scene_tree_box = (Rectangle){ 0.0f, 0.0f, GetScreenWidth() / 4.0f, GetScreenHeight() / 1.0f };
	DrawRectangleRec(scene_tree_box, (Color){ 200, 200, 200, 200 });

	int y = 0;
	editor_iterate_scene(s->root, 0, &y);
}

void editor_iterate_scene(Shape *pos, int depth, int *y) {
	if (pos == NULL) {
		editor_draw_tree_node(depth, *y, pos);
		return;
	}

	(*y)++;
	switch (pos->type) {
		case stPRIMITIVE:
			editor_draw_tree_node(depth, *y, pos);
			break;
		case stGROUP:
			editor_draw_tree_node(depth, *y, pos);
			editor_iterate_scene(pos->g.a, depth + 1, y);
			editor_iterate_scene(pos->g.b, depth + 1, y);
			break;
		case stWRAPPER:
			editor_draw_tree_node(depth, *y, pos);
			editor_iterate_scene(pos->w.shape, depth + 1, y);
			break;
		default:
			die("invalid shape type %d", pos->type);
	}
}

void editor_draw_tree_node(int x, int y, Shape *s) {
	Rectangle text_box = (Rectangle){ x * 20.0f + 20.0f, y * 20.0f, 200.0f, 200.0f};
	char* shape_name;
	switch (s->type) {
		case stPRIMITIVE: shape_name = "PRIMT"; break;
		case stGROUP    : shape_name = "GROUP"; break;
		case stWRAPPER  : shape_name = "WRAPP"; break;
	}
	GuiLabel(text_box, shape_name);
}
