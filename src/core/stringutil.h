#ifndef DF_CORE_STRING_UTIL
#define DF_CORE_STRING_UTIL
#include <stdarg.h>
#include <string.h>

char* saprintf(const char* format, ...);
char* vsaprintf(const char* format, va_list args);

#endif
