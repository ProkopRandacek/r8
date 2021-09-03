#include "_raygui.h"
#include <raylib.h>
#include <stdio.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

bool r8_open = false;
bool scene_open = false;
bool about_open = false;

static bool build_info_message_box = false;

void editor_draw_toolbar(void) {
	Rectangle toolbar_bg = (Rectangle){ 0.0f, 0.0f, GetScreenWidth(), UNIT };
	GuiPanel(toolbar_bg);

	bool clicked;

	const float r8_width = 200.0f;

	Rectangle menu_r8 = (Rectangle){ 0.0f, 0.0f, r8_width, UNIT };
	clicked = GuiButton(menu_r8, "R8");
	if (clicked) r8_open = !r8_open;
	if (r8_open) {
		scene_open = about_open = false;

		Rectangle rec;

		rec = (Rectangle){ 0.0f, UNIT, r8_width, UNIT };
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#159#Quit");
		if (clicked) CloseWindow();

		rec.y += UNIT;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#7#Compile");
		if (clicked) printf("Compile placeholder!\n");
	}

	const float scene_width = 200.0f;

	Rectangle menu_scene = (Rectangle){ r8_width, 0.0f, scene_width, UNIT };
	clicked = GuiButton(menu_scene, "Scene");
	if (clicked) scene_open = !scene_open;
	if (scene_open) {
		r8_open = about_open = false;

		Rectangle rec;

		rec = (Rectangle){ r8_width, UNIT, scene_width, UNIT };
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#5#Open");

		rec.y += UNIT;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#6#Save");

		rec.y += UNIT;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#22#Rename");

		rec.y += UNIT;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#8#New");

		rec.y += UNIT;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#9#Delete");
	}

	const float about_width = 200.0f;

	Rectangle menu_about = (Rectangle){ r8_width + scene_width, 0.0f, about_width, UNIT };
	clicked = GuiButton(menu_about, "About");
	if (clicked) about_open = !about_open;
	if (about_open) {
		r8_open = scene_open = false;

		Rectangle rec;

		rec = (Rectangle){ r8_width + scene_width, UNIT, about_width, UNIT };
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "Build info");
		if (clicked) {
			build_info_message_box = true;
			about_open = false;
		}

		rec.y += UNIT;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "Github link");

		rec.y += UNIT;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "Licenses");
	}

	if (build_info_message_box) {
		float box_width = 350.0f;
		float box_height = 100.0f;
		Rectangle box = (Rectangle){
			(GetScreenWidth() - box_width) / 2.0f,
				(GetScreenHeight() - box_height) / 2.0f,
				box_width,
				box_height
		};
		int i = GuiMessageBox(box, "Build info", "R8 " R8_VERSION " (" R8_COMMIT_HASH ")", "OK");
		if (i != -1) build_info_message_box = false;
	}
}

