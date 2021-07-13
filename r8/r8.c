#include <ucw/lib.h>

#include <raylib.h>

#include <stdio.h>

#include "common.h"
#include "autoconf.h"

int main(int argc, char* argv[]) {
	log_init(argv[0]);
	msg(L_INFO, "start");

	const int width = 800;
	const int height = 450;

	InitWindow(width, height, "r8");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
