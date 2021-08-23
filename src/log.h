// vim: ft=c
#ifndef LOG_H
#define LOG_H

#include "common.h"

#include <stdarg.h>

void raylib_log_to_libucw_log(int log_level, const char *text, va_list args);

#endif
