#include <ucw/lib.h>

#include <raylib.h>

#include <stdio.h>

#include "common.h"
#include "autoconf.h"

#define GLSL_VERSION 330

void raylib_log_to_libucw_log(int log_level, const char *text, va_list args);
void raylib_log_to_libucw_log(int raylib_log_level, const char *text, va_list args) {
	int libucw_log_level;
	switch (raylib_log_level) {
		case LOG_DEBUG:   libucw_log_level = L_DEBUG; break;
		case LOG_INFO:    libucw_log_level = L_INFO;  break;
		case LOG_WARNING: libucw_log_level = L_WARN;  break;
		case LOG_ERROR:   libucw_log_level = L_ERROR; break;
	}
	vmsg(libucw_log_level, text, args);
}

int main(int argc, char* argv[]) {
	log_init(argv[0]);
	msg(L_INFO, "START");

	SetTraceLogCallback(raylib_log_to_libucw_log);

	int screenWidth = 800;
	int screenHeight = 450;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "r8");

	Shader shader = LoadShader(0, TextFormat("assets/shader.glsl", GLSL_VERSION));
	int resolutionLoc = GetShaderLocation(shader, "iResolution");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		if (IsWindowResized())
		{
			screenWidth = GetScreenWidth();
			screenHeight = GetScreenHeight();
			float resolution[2] = { (float)screenWidth, (float)screenHeight };
			SetShaderValue(shader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
		}

		BeginDrawing(); {
			BeginShaderMode(shader); {
				DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
			} EndShaderMode();
		} EndDrawing();
	}

	UnloadShader(shader);
	CloseWindow();

	return 0;
}

