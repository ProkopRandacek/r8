#include "_raygui.h"
#include <raylib.h>
#include <stdio.h>

#include "alloc.h"
#include "editor.h"
#include "log.h"
#include "scene.h"

extern bool r8_open, scene_open, about_open; // from editor_toolbar.c
extern Vector2 editor_layout; // from editor.c

Shape* selected_shape;

static Rectangle box;
static Vector2 scroll;
static Rectangle view;
static Shape *to_del = NULL;

static int tree_height = 0;
static Shape *parent = NULL;
static bool second_child;
static bool update_scene;

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
		parent = NULL;
		iterate_scene(s->root, 0, &y);

		Rectangle ver_box = (Rectangle){
			GetScreenWidth() - UNIT * 12,
				GetScreenHeight() - GuiGetStyle(DEFAULT, TEXT_SIZE),
				UNIT * 10,
				GuiGetStyle(DEFAULT, TEXT_SIZE)
		};
		GuiLabel(ver_box, "R8 " R8_VERSION " (" R8_COMMIT_HASH ")");
	} EndScissorMode();

	if (to_del != NULL)  {
		scene_delete_shape(s, to_del);
		to_del = NULL;
	}
	if (update_scene) {
		scene_on_tree_update(s);
		update_scene = false;
	}
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
					parent = pos;
					second_child = false;
					iterate_scene(pos->g.a, depth + 1, y);
					parent = pos;
					second_child = true;
					iterate_scene(pos->g.b, depth + 1, y);
				}
				break;
			case stWRAPPER:
				editor_draw_tree_node(depth, *y, pos);
				parent = pos;
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
		(x + 3) * UNIT,
		  y * UNIT + box.y + scroll.y,
		  box.width - (x + 1.5f) * UNIT,
		  UNIT
	};
	if (s != NULL && s == selected_shape) GuiPanel(text_box);

	if (s != NULL) {
		if (!(r8_open || about_open || scene_open)) {
			bool text_clicked = GuiLabelButton(text_box, s == NULL ? "NULL" : s->name);
			if (text_clicked) {
				if (s == selected_shape)
					selected_shape = NULL;
				else
					selected_shape = s;
			}
		} else
			GuiLabel(text_box, s->name);
	}

	// Delete icon
	if (s != NULL && x != 0) { // hacky way to detect root shape (depth == 0)
		Rectangle del_box = (Rectangle){ 0, y * UNIT + box.y + scroll.y, UNIT, UNIT };
		if (!(r8_open || about_open || scene_open)) {
			bool clicked = GuiButton(del_box, "#143#");
			if (clicked) {
				if (s == selected_shape) selected_shape = NULL;
				to_del = s;
			}
		} else {
			GuiLabel(del_box, "D");
		}
	}

	// Visible icon
	if (s != NULL) {
		Rectangle del_box = (Rectangle){ UNIT, y * UNIT + box.y + scroll.y, UNIT, UNIT };
		bool clicked = GuiButton(del_box, s->visible ? "#44#" : "#45#");
		if (clicked) {
			s->visible = !s->visible;
			update_scene = true;
		}
	}

	// collapse arrow next to groups && wrappers
	if (s != NULL && (s->type == stGROUP || s->type == stWRAPPER)) {
		Rectangle arrow_box = (Rectangle){
			(x + 2) * UNIT,
				y * UNIT + box.y + scroll.y,
				UNIT,
				UNIT
		};

		bool arrow_clicked = GuiButton(arrow_box, s->g.collapsed ? "#119#" : "#120#");
		if (arrow_clicked) {
			if (s->type == stGROUP) s->g.collapsed = !s->g.collapsed;
			else if (s->type == stWRAPPER) msg("wrapper collapse placeholder");
		}
	}

	// New Shape buttons for NULL things
	if (s == NULL) {
		bool clicked;

		Rectangle shape_box = (Rectangle){ (x + 3) * UNIT, y * UNIT + box.y + scroll.y, UNIT, UNIT };
		clicked = GuiButton(shape_box, "S");
		if (clicked) {
			Shape *s = sphere_new("noname", (Vector3){0, 5, 0}, (Vector4){0, 0, 0, 0}, 1.0f);
			if (parent->type == stGROUP) {
				if (second_child)
					parent->g.b = s;
				else
					parent->g.a = s;
			} else if (parent->type == stWRAPPER) {
				parent->w.shape = s;
			}
			update_scene = true;
		}

		Rectangle group_box = (Rectangle){ (x + 4) * UNIT, y * UNIT + box.y + scroll.y, UNIT, UNIT };
		clicked = GuiButton(group_box, "G");
		if (clicked) {
			Shape *s = group_new("noname", NULL, NULL, gtUNION, 0.5f);
			if (parent->type == stGROUP) {
				if (second_child)
					parent->g.b = s;
				else
					parent->g.a = s;
			} else if (parent->type == stWRAPPER) {
				parent->w.shape = s;
			}
			update_scene = true;
		}

		Rectangle wrapper_box = (Rectangle){ (x + 5) * UNIT, y * UNIT + box.y + scroll.y, UNIT, UNIT };
		clicked = GuiButton(wrapper_box, "W");
		if (clicked) {
			msg("new wrapper placeholder");
		}
	}
}

