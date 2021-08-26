#include "log.h"

#include <raylib.h>

void raylib_log_to_libucw_log(int raylib_log_level, const char *text, va_list args) {
#ifdef R8_DEBUG
	int libucw_log_level;
	switch (raylib_log_level) {
		case LOG_DEBUG:   libucw_log_level = L_DEBUG; break;
		case LOG_INFO:    libucw_log_level = L_INFO;  break;
		case LOG_WARNING: libucw_log_level = L_WARN;  break;
		case LOG_ERROR:   libucw_log_level = L_ERROR; break;
		default:          libucw_log_level = L_ERROR; break;
	}
	vmsg((unsigned int)libucw_log_level, text, args);
#endif
}

