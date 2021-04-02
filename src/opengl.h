// vim: filetype=c
#ifndef OPENGL_H
#define OPENGL_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "fileio.h"
#include "scene/shaderGen.h"

typedef struct GL {
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int s;
	GLFWwindow* window;
} GL;

// init GL stuff
void initOGL(Scene s);

// render fragment shader basically
void renderOGL(void);

// exit GL stuff
void exitOGL(void);

// take a screenshot and write it to bmp file
void screenshot(void);

// callback functions
void onError(int error, const char* description);
void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
void resize(GLFWwindow* window, int width, int height);
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLsizei length, const char *message, const void *userParam);
#endif
