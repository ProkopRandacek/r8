// vim: ft=c
#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>

/**
 * @brief Allocate memory, die if malloc fails.
 */
void *xmalloc(size_t size);

/**
 * @brief Allocate zeroed memory, die if malloc fails.
 */
void *xmalloc_zero(size_t size);

/**
 * @brief Free memory.
 */
void xfree(void *ptr);

/**
 * @brief Reallocate memory, die if realloc fails.
 */
void *xrealloc(void *old, size_t size);

#endif
