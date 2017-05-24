#define LOG_CATEGORY "Core"
#include "check.h"

bool _check(const char* file, uint32 line, const char* category, bool value, log_level level, const char* message, ...) {
	va_list args;
	va_start(args, message);
    if(!value)
    {
        _log_va(file, line, category, level, message, args);
    }
	va_end(args);

    return !value;
}
