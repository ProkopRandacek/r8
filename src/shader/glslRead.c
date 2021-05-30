#include "glslRead.h"

char* readGlsl(const char* name) {
	char filename[64] = { 0 };
	strcat(filename, "./glslPieces/");
	strcat(filename, name);
	strcat(filename, ".glsl");

	FILE *fp;
	fp = fopen(filename, "r");

	if (!fp) {
		printf("error opening file \"%s\"\n", filename);
		exit(1);
	}

	fseek(fp, 0L, SEEK_END);
	long int fileSize = ftell(fp);

	char* content = calloc(1, fileSize + 1);

	fseek(fp, 0L, SEEK_SET);

	fread(content, fileSize, 1, fp);

	return content;
}

char* readSDF(const char* name) {
	char filename[64] = { 0 };
	strcat(filename, "sdfs/");
	strcat(filename, name);
	return readGlsl(filename);
}
