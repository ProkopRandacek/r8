#include <ucw/lib.h>

#include <raylib.h>

#include "log.h"
#include "main.h"
#include "scene.h"

int main(int argc, char* argv[]) {
	log_init(argv[0]);
	msg(L_INFO, "START");
	if (argc != 1) die("usage: `./r8`");
	SetTraceLogCallback(raylib_log_to_libucw_log);

	InitWindow(800, 450, "r8");
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);

	Scene *s = scene_new();

	while (!WindowShouldClose()) {
		scene_update(s);

		BeginDrawing(); {
			BeginShaderMode(s->shader); {
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
			} EndShaderMode();
			DrawFPS(10, 10);
		} EndDrawing();
	}

	scene_destroy(s);
	CloseWindow();

	return 0;
}

