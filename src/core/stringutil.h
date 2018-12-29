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
// Split str_a with delimiter str_b, returning an array of all (non-zero length) substrings
char** nstrsplit(const char* str_a, const char* str_b, uint16* count);
// Duplicate a string, or return NULL if the string is NULL
#define nstrdup(str) (str ? strdup(str) : NULL)
// Duplicate a string to a given length, or return NULL if the string is NULL
#define nstrndup(str, len) (str ? strndup(str, len) : NULL)
// Check if a string is null or empty
#define nstrempty(str) (nstrlen(str) == 0)
// Compare two strings and return true if their content matches, accepting NULL as a valid string value
#define nstreq(str_a, str_b) (nstrcmp(str_a, str_b) == COMPARE_EQUAL_TO)
// Compare two strings to a given length and return true if their content matches, accepting NULL as a valid string value
#define nstrneq(str_a, str_b, len) (nstrncmp(str_a, str_b, len) == COMPARE_EQUAL_TO)
// Get the length of a given string, defaulting to 0 if it's null
#define nstrlen(str) (str ? strlen(str) : 0)
// Get the length of a given string up to len, defaulting to 0 if it's null
#define nstrnlen(str, len) (str ? strnlen(str, len) : 0)

// Check if a string contains a valid integer value
bool aisi(const char* str);
// Check if a string contains a valid floating-point value
bool aisf(const char* str);

#endif
