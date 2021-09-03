#include "_raygui.h"
#include <raylib.h>
#include <stdio.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

extern Shape* selected_shape;
extern Vector2 editor_layout;

void editor_draw_properties() {
	Rectangle box = (Rectangle){
		0.0f,
			editor_layout.y + UNIT,
			editor_layout.x + UNIT,
			GetScreenHeight() - editor_layout.y
	};

	GuiPanel(box);

	if (selected_shape != NULL) {
		Rectangle text = (Rectangle){ box.x, box.y, box.width, UNIT };
		GuiLabel(text, selected_shape->name);
	}
}

