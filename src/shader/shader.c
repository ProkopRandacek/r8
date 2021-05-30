#include "shader.h"

#include "shaderGen.h"
#include "debug.h"

unsigned int bindedShader;

unsigned int shd(const char* vertShdSource, const char* fragShdSource) {
	int success;
	char infoLog[2048];
	unsigned int vertShd = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShd, 1, &vertShdSource, NULL);
	glCompileShader(vertShd);
	glGetShaderiv(vertShd, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertShd, 512, NULL, infoLog);
		printf("Error while compiling vertex shader\n%s\n", infoLog);
		exit(1);
	}
	char l[61]; sprintf(l, "GL - SHADER - vertex shader compiled (%ld)", strlen(vertShdSource)); dprint(l);

	unsigned int fragShd = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShd, 1, &fragShdSource, NULL);
	glCompileShader(fragShd);
	glGetShaderiv(fragShd, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShd, 512, NULL, infoLog);
		printf("Error while compiling fragment shader\n%s\n", infoLog);
		exit(1);
	}
	sprintf(l, "GL - SHADER - fragment shader compiled (%ld)", strlen(fragShdSource)); dprint(l);

	unsigned int s = glCreateProgram();
	glAttachShader(s, vertShd);
	glAttachShader(s, fragShd);
	glLinkProgram(s);
	glGetProgramiv(s, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(s, 512, NULL, infoLog);
		printf("Error while linking shaders\n%s\n", infoLog);
		exit(1);
	}
	dprint("GL - SHADER - shaders linked");

	glDeleteShader(vertShd);
	glDeleteShader(fragShd);
	return s;
}

unsigned int createShader(Scene* s) {
	char* vert = createVertSource();
	char* frag = createFragSource(s);
	return shd(vert, frag);
}

void activateShader(unsigned int s) {
	bindedShader = s;
	glUseProgram(s);
}

// single values
void shdSetInt  (const char* name, int value)    { glUniform1i(glGetUniformLocation(bindedShader, name), value); }
void shdSetFloat(const char* name, float value)  { glUniform1f(glGetUniformLocation(bindedShader, name), value); }
void shdSetIVec2(const char* name, int x, int y) { glUniform2i(glGetUniformLocation(bindedShader, name), x, y); }

// arrays
void shdSetFloatArray(const char* name, int count, float* values) { glUniform1fv(glGetUniformLocation(bindedShader, name), count, values); }
void shdSetIntArray  (const char* name, int count, int*   values) { glUniform1iv(glGetUniformLocation(bindedShader, name), count, values); }
void shdSetVec3Array (const char* name, int count, float* values) { glUniform3fv(glGetUniformLocation(bindedShader, name), count, values); }
void shdSetVec4Array (const char* name, int count, float* values) { glUniform4fv(glGetUniformLocation(bindedShader, name), count, values); }

