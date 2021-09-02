#ifndef UTIL_H
#define UTIL_H

#include "common.h"

/**
 * @brief Prints a message and dies.
 */
void die(const char *, ...) NONRET FORMAT_CHECK(printf,1,2);

/**
 * @brief Prints a message.
 */
void msg(const char *fmt, ...) FORMAT_CHECK(printf,1,2);

void vdie(const char *fmt, va_list args) NONRET;
void vmsg(const char *fmt, va_list args);

#endif

