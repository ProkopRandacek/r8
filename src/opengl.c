#include "opengl.h"

#include "debug.h"
#include "fileio.h"
#include "scene/camera.h"
#include "shader/shader.h"

int w = 1000;
int h = 1000;

GL* gl;
unsigned int frameCount = 0;

void initOGL() {
	dprint("GL START");
	gl = malloc(sizeof(GL));

	// glfw init
	if (!glfwInit()) eprint("GL - GLFW init failed\n");
	dprint("GL - glfw init");

	char title[32] = "R8 - ";
	strcat(title, BUILD_NAME); // build name is defined from makefile

	// create window
	gl->window = glfwCreateWindow(w, h, title, NULL, NULL);
	if (!gl->window) eprint("GL - Window creation failed\n");

	dprint("GL - window init");

	glfwMakeContextCurrent(gl->window);

	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0) {
		eprint("GL - Failed to initialize OpenGL context");
	}
	char g[128];
	sprintf(g, "GL - Loaded OpenGL %d.%d", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version)); dprint(g);
	sprintf(g, "GL - GL_RENDERER: %s",                 glGetString(GL_RENDERER));                     dprint(g);
	sprintf(g, "GL - GL_VERSION: %s",                  glGetString(GL_VERSION));                      dprint(g);
	sprintf(g, "GL - GL_SHADING_LANGUAGE_VERSION: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));     dprint(g);

	glViewport(10, 10, w, h);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	glfwSwapInterval(0);

	dprint("GL - window setup");

	// triangles
	float vertices[] = { 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f };
	unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };

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

	glBindVertexArray(gl->VAO);

	glfwSetErrorCallback(onError);
	//glfwSetKeyCallback(gl->window, onKey);
	glfwSetFramebufferSizeCallback(gl->window, resize);
	//glfwSetKeyCallback(gl->window, onKey);

	glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	dprint("GL DONE");
}

void renderOGL() {
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// swap buffers
	glfwSwapBuffers(gl->window);
	glfwPollEvents();
}

void screenshot() {
	frameCount++;

	float* pixels = malloc(sizeof(float) * (long unsigned int)(w * h * 3));

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
	shdSetIVec2("resolution", width, height);
	w = width;
	h = height;
}

