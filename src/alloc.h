#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>

void *xmalloc(size_t size);
void *xmalloc_zero(size_t size);
void xfree(void *ptr);
void *xrealloc(void *old, size_t size);

#endif
