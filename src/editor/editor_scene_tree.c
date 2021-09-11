#include "_raygui.h"
#include <raylib.h>
#include <stdio.h>

#include "alloc.h"
#include "editor.h"
#include "log.h"
#include "scene.h"

/**
 * We disable some buttons if a toolbar is open.
 * Because raylib triggers clicks even if the clicked
 * element is under another element.
 *
 * (so clicking on a toolbar button can also trigger click
 * of unrelated button under it in the scene editor)
 */
extern bool r8_open, scene_open, about_open; // from editor_toolbar.c

static Rectangle box;
static Vector2 scroll;
static Rectangle view;
static Shape *to_del = NULL;

static int tree_height = 0;
/**
 * Shapes don't have pointers to their parents because
 * it is not really needed except for here.
 *
 * This variable is used to figure out to which shape attach a
 * new shape when creating from shapes in editor.
 */
static Shape *parent = NULL;

/**
 * When creating shapes inside a group, only knowing the parent isn't
 * enough
 *
 * We need to know if the user wants to create shape in the first
 * or the second slot of this group. That's what this variable
 * is for.
 */
static bool second_child;

/**
 * Scene updates are done at the end of a frame if anybody set this flag.
 *
 * (Shape created, deleted, edited, ...)
 */
static bool update_scene;

static void iterate_scene(Editor *e, Shape *pos, int depth, int *y);
static void draw_node(Editor *e, Shape *s, int x, int y);

void editor_draw_scene_tree(Editor *e) {
	box               = (Rectangle){ 0, 2 * e->unit, e->editor_layout.x + e->unit, e->editor_layout.y - e->unit };
	Rectangle content = (Rectangle){ 0, 0, e->editor_layout.x + 0.5 * e->unit, MAX((tree_height + 1) * e->unit, e->editor_layout.y - e->unit * 1.1f)};

	Rectangle title = (Rectangle){ e->unit, e->unit, box.width - e->unit, e->unit };
	GuiPanel(title);
	GuiLabel(title, "Scene Editor");

	view = GuiScrollPanel(box, content, &scroll);
	Rectangle back_button = (Rectangle){ 0.0f, e->unit, e->unit, e->unit };
	bool back_clicked = GuiButton(back_button, "#114#");
	if (back_clicked)
		msg("scene select placeholder");

	BeginScissorMode(view.x, view.y, view.width, view.height); {
		int y = 0;
		tree_height = 0;
		parent = NULL;
		iterate_scene(e, e->scene->root, 0, &y);

		Rectangle ver_box = (Rectangle){
			GetScreenWidth() - e->unit * 12,
				GetScreenHeight() - GuiGetStyle(DEFAULT, TEXT_SIZE) - e->unit,
				e->unit * 10,
				GuiGetStyle(DEFAULT, TEXT_SIZE)
		};
		GuiLabel(ver_box, "R8 " R8_VERSION " (" R8_COMMIT_HASH ")");
	} EndScissorMode();

	if (to_del != NULL)  {
		scene_delete_shape(e->scene, to_del);
		to_del = NULL;
	}
	if (update_scene) {
		scene_on_tree_update(e->scene);
		update_scene = false;
	}
}

void iterate_scene(Editor *e, Shape *pos, int depth, int *y) {
	(*y)++;
	if (pos == NULL) {
		draw_node(e, pos, depth, *y);
	} else {
		switch (pos->type) {
			case stPRIMITIVE:
				draw_node(e, pos, depth, *y);
				break;
			case stGROUP:
				draw_node(e, pos, depth, *y);
				if (!pos->g.collapsed) {
					parent = pos;
					second_child = false;
					iterate_scene(e, pos->g.a, depth + 1, y);
					parent = pos;
					second_child = true;
					iterate_scene(e, pos->g.b, depth + 1, y);
				}
				break;
			case stWRAPPER:
				draw_node(e, pos, depth, *y);
				parent = pos;
				iterate_scene(e, pos->w.shape, depth + 1, y);
				break;
			default:
				die("invalid shape type %d", pos->type);
		}
	}
}

void draw_node(Editor *e, Shape *s, int x, int y) {
	tree_height++;
	// the node clickable text
	Rectangle text_box = (Rectangle){
		(x + 3) * e->unit,
		  y * e->unit + box.y + scroll.y,
		  box.width - (x + 1.5f) * e->unit,
		  e->unit
	};
	if (s != NULL && s == e->selected_shape) GuiPanel(text_box);

	if (s != NULL) {
		if (!(r8_open || about_open || scene_open)) {
			bool text_clicked = GuiLabelButton(text_box, s == NULL ? "NULL" : s->name);
			if (text_clicked) {
				if (s == e->selected_shape)
					e->selected_shape = NULL;
				else
					e->selected_shape = s;
			}
		} else
			GuiLabel(text_box, s->name);
	}

	// Delete icon
	if (s != NULL && x != 0) { // hacky way to detect root shape (depth == 0)
		Rectangle del_box = (Rectangle){ 0, y * e->unit + box.y + scroll.y, e->unit, e->unit };
		bool clicked = GuiButton(del_box, "#143#");
		if (clicked) {
			if (s == e->selected_shape) e->selected_shape = NULL;
			to_del = s;
		}
	}

	// Visible icon
	if (s != NULL) {
		Rectangle del_box = (Rectangle){ e->unit, y * e->unit + box.y + scroll.y, e->unit, e->unit };
		bool clicked = GuiButton(del_box, s->visible ? "#44#" : "#45#");
		if (clicked) {
			s->visible = !s->visible;
			update_scene = true;
		}
	}

	// collapse arrow next to groups && wrappers
	if (s != NULL && (s->type == stGROUP || s->type == stWRAPPER)) {
		Rectangle arrow_box = (Rectangle){ (x + 2) * e->unit, y * e->unit + box.y + scroll.y, e->unit, e->unit };
		bool arrow_clicked = GuiButton(arrow_box, s->g.collapsed ? "#119#" : "#120#");
		if (arrow_clicked) {
			if (s->type == stGROUP) s->g.collapsed = !s->g.collapsed;
			else if (s->type == stWRAPPER) msg("wrapper collapse placeholder");
		}
	}

	// New Shape buttons for NULL things
	if (s == NULL) {
		bool clicked;

		Shape *s = NULL;

		Rectangle shape_box = (Rectangle){ (x + 3) * e->unit, y * e->unit + box.y + scroll.y, e->unit, e->unit };
		clicked = GuiButton(shape_box, "S");
		if (clicked) s = sphere_new("noname", (Vector3){0, 5, 0}, (Vector4){0, 0, 0, 0}, 1.0f);

		Rectangle group_box = (Rectangle){ (x + 4) * e->unit, y * e->unit + box.y + scroll.y, e->unit, e->unit };
		clicked = GuiButton(group_box, "G");
		if (clicked) s = group_new("noname", NULL, NULL, gtUNION, 0.5f);

		Rectangle wrapper_box = (Rectangle){ (x + 5) * e->unit, y * e->unit + box.y + scroll.y, e->unit, e->unit };
		clicked = GuiButton(wrapper_box, "W");
		if (clicked) msg("new wrapper placeholder");

		if (s != NULL) {
			if (parent->type == stGROUP) {
				if (second_child) parent->g.b = s;
				else              parent->g.a = s;
			} else if (parent->type == stWRAPPER) {
				parent->w.shape = s;
			}
			update_scene = true;
		}
	}
}

