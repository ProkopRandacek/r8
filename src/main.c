#include <raylib.h>
#include <microui.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "r8.h"

int main(int argc, char* argv[]) {
	if (argc == 2 && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v"))) {
		puts("R8 " R8_VERSION " (" R8_COMMIT_HASH ")\nCompiled at " __DATE__ " " __TIME__);
		exit(0);
	} else if (argc > 1) {
		die("usage: r8 [-v|--version]");
	}

	r8_init();

#ifdef R8_EDITOR
	r8_run_editor();
#endif

	return 0;
}

