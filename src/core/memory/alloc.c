// Log category, used to filter logs
#define LOG_CATEGORY "Core"

#include "log.h"
#include <stdlib.h>

#include "alloc.h"

// Allocates memory with malloc, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the salloc macro instead.
void* _salloc(const char* file, unsigned line, size_t size)
{
    if(size == 0)
        _log(file, line, LOG_CATEGORY, LOG_FATAL, "salloc: Can't allocate a pointer with 0 bytes");

    void* data = malloc(size);

    if(data == NULL)
        _log(file, line, LOG_CATEGORY, LOG_FATAL, "salloc: Failed to allocate memory");

    return data;
}

// Allocates memory with calloc, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the scalloc macro instead.
void* _scalloc(const char* file, unsigned line, size_t nmemb, size_t size)
{
    if(nmemb == 0 || size == 0)
        _log(file, line, LOG_CATEGORY, LOG_FATAL, "scalloc: Can't allocate a pointer with 0 bytes");

    void* data = calloc(nmemb, size);

    if(data == NULL)
        _log(file, line, LOG_CATEGORY, LOG_FATAL, "scalloc: Failed to allocate memory");

    return data;
}

// Reallocates memory with realloc, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the resalloc macro instead.
void* _resalloc(const char* file, unsigned line, void* ptr, size_t size)
{
    if(size == 0) {
        _log(file, line, LOG_CATEGORY, LOG_ERROR, "resalloc: Size of 0 bytes received. Calling sfree...");
        _sfree(file, line, ptr);

        return NULL;
    }

    void* data = realloc(ptr, size);

    if(data == NULL)
        _log(file, line, LOG_CATEGORY, LOG_FATAL, "resalloc: Failed to reallocate memory");

    return data;
}

// Frees memory with free, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the sfree macro instead.
void _sfree(const char* file, unsigned line, void* ptr)
{
    if(ptr == NULL)
        _log(file, line, LOG_CATEGORY, LOG_WARNING, "sfree: Can't free a null pointer");
    else
        free(ptr);
}

#undef LOG_CATEGORY
