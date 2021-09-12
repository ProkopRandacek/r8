#include <raylib.h>
#include <microui.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "inconsolata.ttf.asset.h"

#include "r8.h"

#include "alloc.h"
#include "editor.h"
#include "log.h"
#include "scene.h"
#include "shapes.h"

void raylib_log() {}

bool r8_should_close = false;

//!< The default font used by R8.
Font def_font;

void r8_init() {
	msg("start");
	SetTraceLogCallback(raylib_log);

	/*
	 * Init Raylib window
	 */
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 800, "R8");
	SetTargetFPS(60);
	def_font = LoadFontFromMemory(".ttf", inconsolata_ttf_asset_bytes, inconsolata_ttf_asset_size, 20, NULL, 95);
}

void r8_exit() {
	CloseWindow();
	msg("bye");
}

#ifdef R8_EDITOR
int text_width(__attribute__((unused))mu_Font font, const char *text, int len) {
	if (len == -1) len = strlen(text);
	return MeasureTextEx(def_font, "i", 20, 0).x * len;
}
int text_height(__attribute__((unused))mu_Font font) { return MeasureTextEx(def_font, "i", 20, 0).y; }

void r8_run_editor() {
	/*
	 * Init Default scene
	 */
	Scene *s = scene_new();

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

	Shape *group_a = group_new("on_floor", sphere, cube, gtUNION, 0.5f);
	Shape *root = group_new("root", group_a, floor, gtUNION, 0.5f);
	s->root = root;

	s->portals[0] = p1;
	s->portals[1] = p2;
	s->portal_count = 2;

	scene_on_tree_update(s);

	/*
	 * Init R8 Editor
	 */
	Editor* e = editor_new(s);

	/*
	 * Init MicroUI
	 */
	mu_Context *ctx = malloc(sizeof(mu_Context));
	mu_init(ctx);
	ctx->text_width = text_width;
	ctx->text_height = text_height;

	/*
	 * Main loop
	 */
	while (!WindowShouldClose() && !r8_should_close) {

		// mouse
		mu_input_mousemove(ctx, GetMouseX(), GetMouseY());
		mu_input_scroll(ctx, 0, GetMouseWheelMove() * -30);

		// text
		static char buf[2];
		sprintf(buf, "%c", GetCharPressed());
		mu_input_text(ctx, buf);

		static bool was_down = false;
		bool is_down = IsKeyDown(KEY_BACKSPACE);
		if (is_down && !was_down)
			mu_input_keydown(ctx, MU_KEY_BACKSPACE);
		else if (!is_down && was_down)
			mu_input_keyup(ctx, MU_KEY_BACKSPACE);
		was_down = is_down;

		// mouse buttons
		if (IsMouseButtonPressed (MOUSE_LEFT_BUTTON)) mu_input_mousedown(ctx, GetMouseX(), GetMouseY(), MU_MOUSE_LEFT);
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) mu_input_mouseup  (ctx, GetMouseX(), GetMouseY(), MU_MOUSE_LEFT);

		scene_tick(e->scene);
		BeginDrawing(); {
			BeginShaderMode(e->scene->shader); {
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0.2 * 255, 0.3 * 255, 0.6 * 255, 255});
			} EndShaderMode();
			editor_draw(ctx, e);
		} EndDrawing();
	}

	xfree(ctx);
	scene_destroy(e->scene);
	editor_destroy(e);
}
#endif

