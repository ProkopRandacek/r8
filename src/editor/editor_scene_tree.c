#include "_raygui.h"
#include <raylib.h>
#include <stdio.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

extern bool r8_open, scene_open, about_open; // from editor_toolbar.c
extern Vector2 editor_layout;

Shape* selected_shape;

static Rectangle tree_box;

void iterate_scene(Shape *pos, int depth, int *y);

void editor_draw_scene_tree(Scene *s) {
	tree_box = (Rectangle){ 0.0f, UNIT, editor_layout.x + UNIT, editor_layout.y };
	GuiPanel(tree_box);

	Rectangle back_button = (Rectangle){ 0.0f, UNIT, UNIT, UNIT };
	bool back_clicked = GuiButton(back_button, "#114#");
	if (back_clicked)
		printf("scene select placeholder\n");

	Rectangle title = (Rectangle){ UNIT, UNIT, tree_box.width - UNIT, UNIT };
	GuiPanel(title);
	GuiLabel(title, "Scene Editor");



	int y = 1;
	iterate_scene(s->root, 0, &y);
}

void iterate_scene(Shape *pos, int depth, int *y) {
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

void editor_draw_tree_node(int x, int y, Shape *s) {
	// the node clickable text
	Rectangle text_box = (Rectangle){
		x * UNIT + UNIT,
		  y * UNIT + tree_box.y,
		  tree_box.width - (x + 1) * UNIT,
		  UNIT
	};
	if (s == selected_shape) GuiPanel(text_box);

	if (!(r8_open || about_open || scene_open)) {
		bool text_clicked = GuiLabelButton(text_box, s->name);
		if (text_clicked) {
			if (s == selected_shape)
				selected_shape = NULL;
			else
				selected_shape = s;
		}
	} else
		GuiLabel(text_box, s->name);

	// collapse arrow next to groups && wrappers
	if (s->type == stGROUP || s->type == stWRAPPER) {
		Rectangle arrow_box = (Rectangle){
			x * UNIT,
			  y * UNIT + tree_box.y,
			  UNIT,
			  UNIT
		};
		if (!(r8_open || about_open || scene_open)) {
			bool arrow_clicked = GuiLabelButton(arrow_box, s->g.collapsed ? "#119#" : "#120#");
			if (arrow_clicked) {
				if (s->type == stGROUP)
					s->g.collapsed = !s->g.collapsed;
				else if (s->type == stWRAPPER)
					printf("wrapper collapse placeholder");
			}
		} else
			GuiLabel(arrow_box, s->g.collapsed ? "#119#" : "#120#");
	}
}

