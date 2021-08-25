// vim: ft=c
#ifndef LOG_H
#define LOG_H

#include "common.h"

#include <stdarg.h>

/** @brief Redirects the raylib log system into the libucw log system */
void raylib_log_to_libucw_log(int log_level, const char *text, va_list args);

#endif
