#include "_raygui.h"
#include <raylib.h>
#include <stdio.h>

#include "editor.h"
#include "alloc.h"

extern Shape* selected_shape;
extern Vector2 editor_layout;

Vector2 scroll;

void editor_draw_properties() {
	Rectangle box = (Rectangle){
		0,
			editor_layout.y + UNIT,
			editor_layout.x + UNIT,
			GetScreenHeight() - editor_layout.y - UNIT
	};

	Rectangle content = (Rectangle){ 0.0f, 0.0f, editor_layout.x + 0.5f * UNIT, 21 * UNIT };

	Rectangle view = GuiScrollPanel(box, content, &scroll);


	BeginScissorMode(view.x, view.y, view.width, view.height); {
		if (selected_shape != NULL) {
			Rectangle text = (Rectangle){ box.x, box.y + scroll.y, box.width, UNIT };

			for (int i = 0; i < 20; i++) {
				text.y += UNIT;
				char str[10];
				sprintf(str, "%d", i);
				GuiLabel(text, str);
			}
		}

		const char version[] = "R8 " R8_VERSION " (" R8_COMMIT_HASH ")";
		Rectangle ver_box = (Rectangle){
			GetScreenWidth() - UNIT * 10.5f,
				GetScreenHeight() - GuiGetStyle(DEFAULT, TEXT_SIZE),
				UNIT * 10,
				GuiGetStyle(DEFAULT, TEXT_SIZE)
		};
		GuiLabel(ver_box, version);
	} EndScissorMode();
}

