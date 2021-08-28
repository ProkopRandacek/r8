#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void vmsg(const char *fmt, va_list args) {
	vprintf(fmt, args);
	putc('\n', stdout);
}

void msg(const char *fmt, ...) {
	va_list args;

	va_start(args, fmt);
	vmsg(fmt, args);
	va_end(args);
}


void vdie(const char *fmt, va_list args) {
	vmsg(fmt, args);
	exit(1);
}

void die(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vdie(fmt, args);
}

