#include <ucw/lib.h>

#include <raylib.h>

#include <stdio.h>

#include "common.h"
#include "autoconf.h"

void raylib_log_to_libucw_log(int log_level, const char *text, va_list args);
void raylib_log_to_libucw_log(int raylib_log_level, const char *text, va_list args) {
	int libucw_log_level;
	switch (raylib_log_level) {
		case LOG_DEBUG:   libucw_log_level = L_DEBUG; break;
		case LOG_INFO:    libucw_log_level = L_INFO;  break;
		case LOG_WARNING: libucw_log_level = L_WARN;  break;
		case LOG_ERROR:   libucw_log_level = L_ERROR; break;
	}
	msg(libucw_log_level, text, args);
}

int main(int argc, char* argv[]) {
	log_init(argv[0]);
	msg(L_INFO, "start");

	SetTraceLogCallback(raylib_log_to_libucw_log);

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

