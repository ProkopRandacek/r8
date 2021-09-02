// taken from the libucw
// http://www.ucw.cz/gitweb/?p=libucw.git;f=ucw/alloc.c;hb=HEAD
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "log.h"

void *xmalloc(size_t size) {
	void *x = malloc(size);
	if (!x)
		die("Cannot allocate %zu bytes of memory", size);
	return x;
}

void *xmalloc_zero(size_t size) {
	void *x = xmalloc(size);
	memset(x, 0, size);
	return x;
}

void xfree(void *ptr) {
	free(ptr);
}

void *xrealloc(void *old, size_t size) {
	/* We assume that realloc(NULL, x) works like malloc(x), which is true with the glibc. */
	void *x = realloc(old, size);
	if (!x && size)
		die("Cannot reallocate %zu bytes of memory", size);
	return x;
}

