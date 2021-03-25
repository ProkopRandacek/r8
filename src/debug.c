#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "debug.h"
#include "opengl.h"
#include "settings.h"

extern GL* gl;

clock_t s;

float t() { return ((float)(clock() - s) / CLOCKS_PER_SEC * 1000.0f); }
void startTime() { s = clock(); }
void dprint(const char* text) { if (DO_DEBUG_LOG) printf("%8.2f ms -- %s\n", t(), text); }
void eprint(const char* text) {
	printf("%8.2f ms -- ERROR: %s\n", t(), text);
	exit(1);
}
