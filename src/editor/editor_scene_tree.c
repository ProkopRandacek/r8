#include "_raygui.h"
#include <raylib.h>
#include <stdio.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

void iterate_scene(Shape *pos, int depth, int *y);

extern bool r8_open, scene_open, about_open; // from editor_toolbar.c
extern Vector2 editor_layout; // from editor.c

Shape* selected_shape;

static Rectangle box;
static Vector2 scroll;
static Rectangle view;

static int tree_height = 0;

void editor_draw_scene_tree(Scene *s) {
	box               = (Rectangle){ 0, 2 * UNIT, editor_layout.x + UNIT, editor_layout.y - UNIT };
	Rectangle content = (Rectangle){ 0, 0, editor_layout.x + 0.5 * UNIT, MAX((tree_height + 1) * UNIT, editor_layout.y - UNIT * 1.1f)};

	Rectangle title = (Rectangle){ UNIT, UNIT, box.width - UNIT, UNIT };
	GuiPanel(title);
	GuiLabel(title, "Scene Editor");

	view = GuiScrollPanel(box, content, &scroll);
	Rectangle back_button = (Rectangle){ 0.0f, UNIT, UNIT, UNIT };
	bool back_clicked = GuiButton(back_button, "#114#");
	if (back_clicked)
		msg("scene select placeholder");

	BeginScissorMode(view.x, view.y, view.width, view.height); {
		int y = 0;
		tree_height = 0;
		iterate_scene(s->root, 0, &y);

		Rectangle ver_box = (Rectangle){
			GetScreenWidth() - UNIT * 12,
				GetScreenHeight() - GuiGetStyle(DEFAULT, TEXT_SIZE),
				UNIT * 10,
				GuiGetStyle(DEFAULT, TEXT_SIZE)
		};
		GuiLabel(ver_box, "R8 " R8_VERSION " (" R8_COMMIT_HASH ")");
	} EndScissorMode();
}

void iterate_scene(Shape *pos, int depth, int *y) {
	(*y)++;
	if (pos == NULL) {
		editor_draw_tree_node(depth, *y, pos);
	} else {
		switch (pos->type) {
			case stPRIMITIVE:
				editor_draw_tree_node(depth, *y, pos);
				break;
			case stGROUP:
				editor_draw_tree_node(depth, *y, pos);
				if (!pos->g.collapsed) {
					iterate_scene(pos->g.a, depth + 1, y);
					iterate_scene(pos->g.b, depth + 1, y);
				}
				break;
			case stWRAPPER:
				editor_draw_tree_node(depth, *y, pos);
				iterate_scene(pos->w.shape, depth + 1, y);
				break;
			default:
				die("invalid shape type %d", pos->type);
		}
	}
}

void editor_draw_tree_node(int x, int y, Shape *s) {
	tree_height++;
	// the node clickable text
	Rectangle text_box = (Rectangle){
		x * UNIT + UNIT,
		  y * UNIT + box.y + scroll.y,
		  box.width - (x + 1.5f) * UNIT,
		  UNIT
	};
	if (s != NULL && s == selected_shape) GuiPanel(text_box);

	if (!(r8_open || about_open || scene_open)) {
		bool text_clicked = GuiLabelButton(text_box, s == NULL ? "NULL" : s->name);
		if (text_clicked) {
			if (s == selected_shape)
				selected_shape = NULL;
			else
				selected_shape = s;
		}
	} else
		GuiLabel(text_box, s == NULL ? "NULL" : s->name);

	// collapse arrow next to groups && wrappers
	if (s != NULL && (s->type == stGROUP || s->type == stWRAPPER)) {
		Rectangle arrow_box = (Rectangle){
			x * UNIT,
			  y * UNIT + box.y + scroll.y,
			  UNIT,
			  UNIT
		};
		if (!(r8_open || about_open || scene_open)) {
			bool arrow_clicked = GuiLabelButton(arrow_box, s->g.collapsed ? ">" : "v"); // the ricon hitboxes are broken
			//bool arrow_clicked = GuiLabelButton(arrow_box, s->g.collapsed ? "#119#" : "#120#");
			if (arrow_clicked) {
				if (s->type == stGROUP) s->g.collapsed = !s->g.collapsed;
				else if (s->type == stWRAPPER) msg("wrapper collapse placeholder");
			}
		} else {
			GuiLabel(arrow_box, s->g.collapsed ? ">" : "v"); // the ricon hitboxes are broken
			//GuiLabel(arrow_box, s->g.collapsed ? "#119#" : "#120#");
		}
	}
}

