#include <raylib.h>
#include <stdio.h>
#include <ucw/lib.h>

#include "log.h"
#include "main.h"

#include "shaders/filled_template.glsl.h"
#include "shaders/template.glsl.h"

#include "shapes/portal.h"

//#include "shader.h"

#define GLSL_VERSION 330

int main(int argc, char* argv[]) {
	log_init(argv[0]);
	msg(L_INFO, "START");
	if (argc != 1) die("usage: `./r8`");

	SetTraceLogCallback(raylib_log_to_libucw_log);

	int screenWidth = 800;
	int screenHeight = 450;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "r8");

	Shader shader = LoadShaderFromMemory(0, filled_template_glsl);

	int resolutionLoc = GetShaderLocation(shader, "resolution");
	int viewEyeLoc    = GetShaderLocation(shader, "viewEye");
	int viewCenterLoc = GetShaderLocation(shader, "viewCenter");

	Camera camera   = { 0 };
	camera.position = (Vector3){ 2.5f, 2.5f, 20.0f };
	camera.target   = (Vector3){ 0.0f, 0.0f, 0.0f };
	camera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy     = 65.0f;

	SetCameraMode(camera, CAMERA_FIRST_PERSON);

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		UpdateCamera(&camera);

		float cameraPos[3]    = { camera.position.x, camera.position.y, camera.position.z };
		float cameraTarget[3] = { camera.target.x,   camera.target.y,   camera.target.z };

		SetShaderValue(shader, viewEyeLoc,    cameraPos, SHADER_UNIFORM_VEC3);
		SetShaderValue(shader, viewCenterLoc, cameraTarget, SHADER_UNIFORM_VEC3);

		if (IsWindowResized()) {
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

