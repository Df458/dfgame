#ifndef DF_CORE_STRING_UTIL
#define DF_CORE_STRING_UTIL
#include "core/types.h"
#include <stdarg.h>
#include <string.h>

// Allocate a new string, based on a given format and arguments
char* saprintf(const char* format, ...);
char* vsaprintf(const char* format, va_list args);

// Compare two strings, accepting NULL as a valid string value
int nstrcmp(const char* str_a, const char* str_b);
// Compare two strings to a given length, accepting NULL as a valid string value
int nstrncmp(const char* str_a, const char* str_b, size_t len);
// Duplicate a string, or return NULL if the string is NULL
#define nstrdup(str) (str ? strdup(str) : NULL)
// Duplicate a string to a given length, or return NULL if the string is NULL
#define nstrndup(str, len) (str ? strndup(str, len) : NULL)
// Compare two strings and return true if their content matches, accepting NULL as a valid string value
#define nstreq(str_a, str_b) (nstrcmp(str_a, str_b) == COMPARE_EQUAL_TO)
// Compare two strings to a given length and return true if their content matches, accepting NULL as a valid string value
#define nstrneq(str_a, str_b, len) (nstrncmp(str_a, str_b, len) == COMPARE_EQUAL_TO)

// Check if a string contains a valid integer value
bool aisi(const char* str);
// Check if a string contains a valid floating-point value
bool aisf(const char* str);

#endif
