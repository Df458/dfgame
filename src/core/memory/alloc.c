// Log category, used to filter logs
#define LOG_CATEGORY "Core"

#include "alloc.h"

#include "check.h"
#include <stdlib.h>

// Allocates memory with malloc, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the salloc macro instead.
void* _salloc(const char* file, unsigned line, const char* function, size_t size)
{
    _check(file, line, function, LOG_CATEGORY, size > 0, LOG_FATAL, "salloc: Can't allocate a pointer with 0 bytes");
    void* data = malloc(size);
    _check(file, line, function, LOG_CATEGORY, data, LOG_FATAL, "salloc: Failed to allocate memory");

    return data;
}

// Allocates memory with calloc, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the scalloc macro instead.
void* _scalloc(const char* file, unsigned line, const char* function, size_t nmemb, size_t size)
{
    _check(file, line, function, LOG_CATEGORY, nmemb > 0 && size > 0, LOG_FATAL, "scalloc: Can't allocate a pointer with 0 bytes");
    void* data = calloc(nmemb, size);
    _check(file, line, function, LOG_CATEGORY, data, LOG_FATAL, "scalloc: Failed to allocate memory");

    return data;
}

// Reallocates memory with realloc, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the resalloc macro instead.
void* _resalloc(const char* file, unsigned line, const char* function, void* ptr, size_t size)
{
    if(_check(file, line, function, LOG_CATEGORY, size > 0, LOG_ERROR, "resalloc: Size of 0 bytes received. Calling sfree...")) {
        _sfree(file, line, function, ptr);
        return NULL;
    }
    void* data = realloc(ptr, size);
    _check(file, line, function, LOG_CATEGORY, data, LOG_FATAL, "resalloc: Failed to allocate memory");

    return data;
}

// Frees memory with free, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the sfree macro instead.
void _sfree(const char* file, unsigned line, const char* function, void* ptr)
{
    if(!_check(file, line, function, LOG_CATEGORY, ptr != NULL, LOG_WARNING, "sfree: Can't free a null pointer")) {
        free(ptr);
    }
}

#undef LOG_CATEGORY
