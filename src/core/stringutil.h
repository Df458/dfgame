#ifndef DF_CORE_STRING_UTIL
#define DF_CORE_STRING_UTIL
#include "core/types.h"
#include <stdarg.h>
#include <string.h>

char* saprintf(const char* format, ...);
char* vsaprintf(const char* format, va_list args);

#define nstrdup(str) (str ? strdup(str) : NULL)

bool aisi(const char* str);
bool aisf(const char* str);

#endif
