#include "_raygui.h"
#include <raylib.h>
#include <stdio.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

static const char* group_type_spelling[] = { "union", "diff", "inters", "blend", "average" };
static const char* shape_type_spelling[] = { "sphere", "cube", "torus", "ctorus", "cyl", "ccone" };

static int last_height = 0;

static Vector2 scroll;

void editor_draw_properties(Editor *e) {
	Rectangle box = (Rectangle){
		0,
			e->editor_layout.y + e->unit,
			e->editor_layout.x + e->unit,
			GetScreenHeight() - e->editor_layout.y - e->unit
	};

	Rectangle content = (Rectangle){ 0.0f, 0.0f, e->editor_layout.x + 0.5f * e->unit, last_height };

	Rectangle view = GuiScrollPanel(box, content, &scroll);

	BeginScissorMode(view.x, view.y, view.width, view.height); {
		if (e->selected_shape != NULL) {
			Rectangle text_box = (Rectangle){ box.x, box.y + scroll.y, box.width, e->unit };
			char str[64];

			sprintf(str, "name: %s", e->selected_shape->name);
			GuiLabel(text_box, str);

			switch (e->selected_shape->type) {
				case stPRIMITIVE:
					text_box.y += e->unit;
					sprintf(str, "type: %s", shape_type_spelling[e->selected_shape->p.type]);
					GuiLabel(text_box, str);

					text_box.y += e->unit;
					sprintf(str, "pos : %.2f, %.2f, %.2f",
							e->selected_shape->p.d[0],
							e->selected_shape->p.d[1],
							e->selected_shape->p.d[2]
					       );
					GuiLabel(text_box, str);

					text_box.y += e->unit;
					sprintf(str, "clr : %.2f, %.2f, %.2f, %.2f",
							e->selected_shape->p.d[3],
							e->selected_shape->p.d[4],
							e->selected_shape->p.d[5],
							e->selected_shape->p.d[6]
					       );
					GuiLabel(text_box, str);
					switch (e->selected_shape->p.type) {
						case ptSPHERE:
							text_box.y += e->unit;
							sprintf(str, "rad : %.2f", prim_get_rad1(&e->selected_shape->p));
							GuiLabel(text_box, str);
							break;
						case ptCUBE:
							text_box.y += e->unit;
							sprintf(str, "scl : %.2f, %.2f, %.2f",
									e->selected_shape->p.d[7],
									e->selected_shape->p.d[8],
									e->selected_shape->p.d[9]
							       );
							GuiLabel(text_box, str);
							break;
						case ptTORUS:
						case ptCTORUS:
						case ptCYL:
						case ptCCONE:
						default:
							die("Trying to view properties of unknown shape type");
					}
					break;
				case stGROUP:
					text_box.y += e->unit;
					sprintf(str, "type: %s", group_type_spelling[e->selected_shape->g.type]);
					GuiLabel(text_box, str);

					text_box.y += e->unit;
					sprintf(str, "k   : %.2f", e->selected_shape->g.k);
					GuiLabel(text_box, str);
					break;
				case stWRAPPER:
					text_box.y += e->unit;
					GuiLabel(text_box, "wrapper wip");
					break;
				default:
					die("Trying to view properties of unknown shape type");
			}
			last_height = text_box.y - (box.y + scroll.y) + e->unit;
		}
	} EndScissorMode();
}

