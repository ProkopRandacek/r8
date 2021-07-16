#include <ucw/lib.h>

#include <raylib.h>

#include <stdio.h>

#include "log.h"
#include "common.h"
#include "autoconf.h"

#define GLSL_VERSION 330

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

	//SetTargetFPS(60);

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
			DrawFPS(10, 10);
		} EndDrawing();
	}

	UnloadShader(shader);
	CloseWindow();

	return 0;
}

