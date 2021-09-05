#include "_raygui.h"
#include <raylib.h>
#include <stdio.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

extern Shape* selected_shape;
extern Vector2 editor_layout;

Vector2 scroll;

const char* group_type_spelling[] = { "union", "diff", "inters", "blend", "average" };
const char* shape_type_spelling[] = { "sphere", "cube", "torus", "ctorus", "cyl", "ccone" };

int last_height = 0;

void editor_draw_properties() {
	Rectangle box = (Rectangle){
		0,
			editor_layout.y + UNIT,
			editor_layout.x + UNIT,
			GetScreenHeight() - editor_layout.y - UNIT
	};

	Rectangle content = (Rectangle){ 0.0f, 0.0f, editor_layout.x + 0.5f * UNIT, last_height };

	Rectangle view = GuiScrollPanel(box, content, &scroll);

	BeginScissorMode(view.x, view.y, view.width, view.height); {
		if (selected_shape != NULL) {
			Rectangle text_box = (Rectangle){ box.x, box.y + scroll.y, box.width, UNIT };
			char str[64];

			sprintf(str, "name: %s", selected_shape->name);
			GuiLabel(text_box, str);

			switch (selected_shape->type) {
				case stPRIMITIVE:
					text_box.y += UNIT;
					sprintf(str, "type: %s", shape_type_spelling[selected_shape->p.type]);
					GuiLabel(text_box, str);

					text_box.y += UNIT;
					sprintf(str, "pos : %.2f, %.2f, %.2f",
							selected_shape->p.d[0],
							selected_shape->p.d[1],
							selected_shape->p.d[2]
					       );
					GuiLabel(text_box, str);

					text_box.y += UNIT;
					sprintf(str, "clr : %.2f, %.2f, %.2f, %.2f",
							selected_shape->p.d[3],
							selected_shape->p.d[4],
							selected_shape->p.d[5],
							selected_shape->p.d[6]
					       );
					GuiLabel(text_box, str);
					switch (selected_shape->p.type) {
						case ptSPHERE:
							text_box.y += UNIT;
							sprintf(str, "rad : %.2f", prim_get_rad1(&selected_shape->p));
							GuiLabel(text_box, str);
							break;
						case ptCUBE:
							text_box.y += UNIT;
							sprintf(str, "scl : %.2f, %.2f, %.2f",
									selected_shape->p.d[7],
									selected_shape->p.d[8],
									selected_shape->p.d[9]
							       );
							GuiLabel(text_box, str);
							break;
					}
					break;
				case stGROUP:
					text_box.y += UNIT;
					sprintf(str, "type: %s", group_type_spelling[selected_shape->g.type]);
					GuiLabel(text_box, str);

					text_box.y += UNIT;
					sprintf(str, "k   : %.2f", selected_shape->g.k);
					GuiLabel(text_box, str);
					break;
				case stWRAPPER:
					text_box.y += UNIT;
					GuiLabel(text_box, "wrapper wip");
					break;
				default:
					die("Trying to view properties of unknown shape type");
			}
			last_height = text_box.y - (box.y + scroll.y) + UNIT;
		}

	} EndScissorMode();
}

