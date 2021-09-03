#include "_raygui.h"

#include <raylib.h>
#include <stdio.h>

#include "common.h"

#include "scene.h"
#include "editor.h"
#include "shapes.h"
#include "log.h"
#include "stack.h"

#include "inconsolata.ttf.asset.h"

//!< The default font used by R8.
Font def_font;

int main(void) {
	msg("START");

	InitWindow(800, 800, "r8");
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);

	Scene *s = scene_new();

	def_font = LoadFontFromMemory(".ttf", inconsolata_ttf_asset_bytes, inconsolata_ttf_asset_size, 32, NULL, 95);

	GuiSetFont(def_font);

	Shape *cube = cube_new("cube",
			(Vector3){-0.5f, 1.0f, 2.0f},
			(Vector4){1.0f, 0.0f, 0.0f, 0.0f},
			(Vector3){1.0f, 1.0f, 1.0f}
			);

	Shape *floor = cube_new("floor",
			(Vector3){0.0f, 0.5f, 0.0f},
			(Vector4){0.0f, 1.0f, 0.0f, 0.0f},
			(Vector3){10.0f, 0.5f, 10.0f}
			);

	Shape *sphere = sphere_new("sphere",
			(Vector3){0.5f, 2.0f, 2.0f},
			(Vector4){0.0f, 0.0f, 1.0f, 0.0f},
			1.0f
			);

	Portal *p1 = portal_new(
			(Vector3){2.0f, 1.0f, 3.0f},
			(Vector3){0.0f, 0.0f, 1.0f},
			(Vector3){0.0f, 1.0f, 0.0f},
			(Vector2){1.0f, 2.0f}
			);

	Portal *p2 = portal_new(
			(Vector3){-2.0f, 1.0f, 3.0f},
			(Vector3){ 1.0f, 0.0f, 0.0f},
			(Vector3){ 0.0f, 1.0f, 0.0f},
			(Vector2){ 1.0f, 2.0f}
			);

	p2->link = p1;
	p1->link = p2;

	Shape *group_a = group_new("on_floor", cube, sphere, gtAVERAGE, 0.5f);
	Shape *root = group_new("root", group_a, floor, gtUNION, 0.5f);

	s->root = root;
	scene_on_tree_update(s);

	s->portals[0] = p1;
	s->portals[1] = p2;
	s->portal_count = 2;

	scene_compile(s);

	float time = 0.0f;

	Editor *editor = editor_new(s);

	while (!WindowShouldClose()) {
		scene_tick(s);

		time += GetFrameTime();
		s->root->g.a->g.k = sinf(time) * 0.5f + 0.5f;
		s->group_changed = true;

		s->portals[0]->pos.x = sinf(time);
		s->portal_changed = true;

		BeginDrawing(); {
			BeginShaderMode(s->shader); {
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
			} EndShaderMode();

			editor_draw(editor);

			DrawFPS(10, 10);
		} EndDrawing();
	}

	scene_destroy(s);
	CloseWindow();
	msg("bye");

	return 0;
}

