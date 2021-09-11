#include "_raygui.h"
#include <raylib.h>
#include <stdio.h>

#include "editor.h"
#include "log.h"

extern bool r8_shoul_close;

bool r8_open = false;
bool scene_open = false;
bool about_open = false;

void editor_draw_toolbar(Editor *e) {
	Rectangle toolbar_bg = (Rectangle){ 0.0f, 0.0f, GetScreenWidth(), e->unit };
	GuiPanel(toolbar_bg);

	bool clicked;

	const float r8_width = e->unit * 6;

	Rectangle menu_r8 = (Rectangle){ 0.0f, 0.0f, r8_width, e->unit };
	clicked = GuiButton(menu_r8, "R8");
	if (clicked) r8_open = !r8_open;
	if (r8_open) {
		scene_open = about_open = false;

		Rectangle rec;

		rec = (Rectangle){ 0.0f, e->unit, r8_width, e->unit };
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#7#Compile");
		if (clicked) msg("Compile placeholder!");

		rec.y += e->unit;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#159#Quit");
		if (clicked) r8_shoul_close = true;
	}

	const float scene_width = e->unit * 6;

	Rectangle menu_scene = (Rectangle){ r8_width, 0.0f, scene_width, e->unit };
	clicked = GuiButton(menu_scene, "Scene");
	if (clicked) scene_open = !scene_open;
	if (scene_open) {
		r8_open = about_open = false;

		Rectangle rec;

		rec = (Rectangle){ r8_width, e->unit, scene_width, e->unit };
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#5#Open");

		rec.y += e->unit;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#6#Save");

		rec.y += e->unit;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#22#Rename");

		rec.y += e->unit;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#8#New");

		rec.y += e->unit;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#9#Delete");
	}

	const float about_width = e->unit * 6;

	Rectangle menu_about = (Rectangle){ r8_width + scene_width, 0.0f, about_width, e->unit };
	clicked = GuiButton(menu_about, "About");
	if (clicked) about_open = !about_open;
	if (about_open) {
		r8_open = scene_open = false;

		Rectangle rec;

		rec = (Rectangle){ r8_width + scene_width, e->unit, about_width, e->unit };
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#0#Github");

		rec.y += e->unit;
		GuiPanel(rec);
		clicked = GuiLabelButton(rec, "#15#Licenses");
	}
}

