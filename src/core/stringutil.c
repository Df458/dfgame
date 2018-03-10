#define LOG_CATEGORY "Core"
#include "stringutil.h"

#include "memory/alloc.h"
#include <stdio.h>
#include <stdarg.h>

char* saprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* final = vsaprintf(format, args);
	va_end(args);

    return final;
}
char* vsaprintf(const char* format, va_list args) {
    if(!format)
        return NULL;

    va_list tempargs;
    size_t length;
	va_copy(tempargs, args);

    length = vsnprintf(0, 0, format, tempargs);
    va_end(tempargs);

	++length;
    char* final = mscalloc(length, char);

    vsnprintf(final, length, format, args);

    return final;
}