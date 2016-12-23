#ifndef DF_CORE_ALLOC
#define DF_CORE_ALLOC

#include <stddef.h>

// Allocates memory with malloc, with error logging.
#define salloc(size) _salloc(__FILE__, __LINE__, size)

// Allocates memory with calloc, with error logging.
#define scalloc(nmemb, size) _scalloc(__FILE__, __LINE__, nmemb, size)

// Reallocates memory with realloc, with error logging.
#define resalloc(ptr, size) _resalloc(__FILE__, __LINE__, ptr, size)

// Frees memory with free, with error logging.
// Also sets ptr to NULL, so that it's harder to double-free.
#define sfree(ptr) { _sfree(__FILE__, __LINE__, ptr); ptr = NULL; }


// Allocates memory with malloc, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the salloc macro instead.
void* _salloc(const char* file, unsigned line, size_t size);

// Allocates memory with calloc, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the scalloc macro instead.
void* _scalloc(const char* file, unsigned line, size_t nmemb, size_t size);

// Reallocates memory with realloc, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the resalloc macro instead.
void* _resalloc(const char* file, unsigned line, void* ptr, size_t size);

// Frees memory with free, with error logging.
// NOTE: Don't call this function unless you want to pass a specific position
//       in code for logging purposes. Use the sfree macro instead.
void _sfree(const char* file, unsigned line, void* ptr);

#endif // DF_CORE_ALLOC
