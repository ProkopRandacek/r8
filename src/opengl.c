#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opengl.h"
#include "debug.h"
#include "camera.h" // the camera needs to be resized with the window

#include "time.h"


int w = 1000;
int h = 1000;

GL* gl;
float* pixels;
unsigned int frameCount = 0;

void initOGL() {
	dprint("GL START");
	gl = malloc(sizeof(GL));

	// glfw init
	if (!glfwInit()) {
		printf("glfw init failed\n");
		exit(1);
	}
	dprint("GL - glfw init");

	char title[32] = "R8 - ";
	strcat(title, BUILD_NAME);

	// create window
	gl->window = glfwCreateWindow(w, h, title, NULL, NULL);
	if (!gl->window) {
		glfwTerminate();
		printf("window creation failed\n");
		exit(1);
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	dprint("GL - window init");

	glfwMakeContextCurrent(gl->window);

	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0) {
		dprint("Failed to initialize OpenGL context");
		exit(1);
	}
	char n[33];
	sprintf(n, "GL - Loaded OpenGL %d.%d", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
	dprint(n);

	glViewport(10, 10, w, h);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	glfwSwapInterval(0);

	dprint("GL - window setup");

	// triangles
	float vertices[] = {
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &gl->VAO);
	glGenBuffers(1, &gl->VBO);
	glGenBuffers(1, &gl->EBO);

	glBindVertexArray(gl->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, gl->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	dprint("GL - buffers done");

	// create shader
	gl->s = shd();
	glUseProgram(gl->s);

	dprint("GL - shaders done");

	glBindVertexArray(gl->VAO);

	glfwSetErrorCallback(onError);
	glfwSetKeyCallback(gl->window, onKey);
	glfwSetFramebufferSizeCallback(gl->window, resize);
	glfwSetKeyCallback(gl->window, onKey);

	glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	dprint("GL DONE");
}

void renderOGL() {
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// swap buffers
	//glClear(GL_COLOR_BUFFER_BIT); saves 0.1 milisecond lol
	glfwSwapBuffers(gl->window);
	glfwPollEvents();
}

void screenshot() {
	frameCount++;

	pixels = malloc(sizeof(float) * (long unsigned int)(w * h * 3));

	char fname[15];
	sprintf(fname, "%04d", frameCount);
	strcat(fname, ".bmp");

	printf("taking a screenshot \"%s\", %dx%d\n", fname, w, h);

	glReadPixels(0, 0, w, h, GL_RGB, GL_FLOAT, pixels);

	writeBMP(fname, pixels, (unsigned int)w, (unsigned int)h);

	free(pixels);
}

void exitOGL() {
	free(gl);
	glDeleteVertexArrays(1, &gl->VAO);
	glDeleteBuffers(1, &gl->VBO);
	glDeleteBuffers(1, &gl->EBO);
	glfwTerminate();
}

// Callback funcs
void onError(int error, const char* description) {
	printf("Glfw error %d: %s\n", error, description);
}

void resize(GLFWwindow* window, int width, int height) {
	if (window != gl->window) return;

	glViewport(0, 0, width, height);
	shdSetIVec2(gl->s, "resolution", width, height);
	float k = 1000.0f;
	setWH((float)width / k, (float)height / k);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void APIENTRY glDebugOutput(GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei length,
		const char *message,
		const void *userParam)
{
	// ignore non-significant error/warning codes
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	printf("---------------\n");
	printf("Debug message (%d): %s\n", id, message);

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             printf("Source: API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("Source: Window System"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("Source: Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION:     printf("Source: Application"); break;
		case GL_DEBUG_SOURCE_OTHER:           printf("Source: Other"); break;
		default: break;
	} printf("\n");

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               printf("Type: Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("Type: Undefined Behaviour"); break;
		case GL_DEBUG_TYPE_PORTABILITY:         printf("Type: Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:         printf("Type: Performance"); break;
		case GL_DEBUG_TYPE_MARKER:              printf("Type: Marker"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          printf("Type: Push Group"); break;
		case GL_DEBUG_TYPE_POP_GROUP:           printf("Type: Pop Group"); break;
		case GL_DEBUG_TYPE_OTHER:               printf("Type: Other"); break;
		default: break;
	} printf("\n");

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         printf("Severity: high"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       printf("Severity: medium"); break;
		case GL_DEBUG_SEVERITY_LOW:          printf("Severity: low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification"); break;
		default: break;
	} printf("\n");
	printf("\n");
}
