// vim: filetype=c
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "fileio.h"

typedef struct GL {
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	GLFWwindow* window;
	shader s;
} GL;

// init GL stuff
void initOGL(void);

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
