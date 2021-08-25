#include <raylib.h>
#include <stdlib.h>

#include "main.h"

#include "log.h"
#include "scene.h"
#include "shapes/shapes.h"

int main(int argc, char* argv[]) {
	log_init(argv[0]);
	msg(L_INFO, "START");
	if (argc != 1) die("usage: `./r8`");
	SetTraceLogCallback(raylib_log_to_libucw_log);

	InitWindow(800, 450, "r8");
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);

	Scene *s = scene_new();

	Shape *cube = cube_new(
			(Vector3){-2.0f, 1.0f, 2.0f},
			(Vector4){0.2f, 0.4f, 0.4f, 1.0f},
			(Vector3){1.0f, 1.0f, 1.0f}
			);

	Shape *floor = cube_new(
			(Vector3){0.0f, 0.5f, 0.0f},
			(Vector4){0.2f, 0.4f, 0.4f, 1.0f},
			(Vector3){10.0f, 0.5f, 10.0f}
			);

	Shape *sphere = sphere_new(
			(Vector3){2.0f, 2.0f, 2.0f},
			(Vector4){0.2f, 0.4f, 0.7f, 1.0f},
			1.0f
			);

	Shape *group_a = group_new(cube, sphere, gtBLEND, 1.0f);
	Shape *root = group_new(group_a, floor, gtBLEND, 1.0f);

	s->root = root;
	scene_on_tree_update(s);

	scene_compile(s);

	while (!WindowShouldClose()) {
		scene_tick(s);

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

