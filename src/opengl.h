// vim: filetype=c
#ifndef OPENGL_H
#define OPENGL_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>

typedef struct GL {
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int s;
	GLFWwindow* window;
} GL;

void initOGL(void);
void renderOGL(void);
void screenshot(void);
void exitOGL(void);
void onError(int error, const char* description);
void resize(GLFWwindow* window, int width, int height);

#endif
