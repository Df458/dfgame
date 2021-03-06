#ifndef DF_CORE_TYPES
#define DF_CORE_TYPES

#include "memory/alloc.h"
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#ifdef WIN32
#define NOMINMAX
#include "windows.h"
#endif

#define INVALID_INDEX -1

#define declare(type, name) typedef type name name;
#define declarep(type, name) typedef type name* name;
#define delegate(type, name, ...) typedef type (*name)(__VA_ARGS__);
#define event(name, ...) typedef void (*name##_func)(__VA_ARGS__, void* user); typedef struct name { name##_func func; void* user; } name;
#define call_event(ev, ...) if(ev && ev->func) { ev->func(__VA_ARGS__, ev->user); }
#define bind_event(bind, ev) { if(bind) sfree(bind); if(ev) { bind = salloc(sizeof(*bind)); bind = memcpy(bind, ev, sizeof(*bind)); } }
#define as_event(type, f, u) &((type){ .func=f, .user=u })

typedef uint8_t uint8;
typedef uint8_t ubyte;
typedef int8_t  int8;
#ifndef WIN32
typedef int8_t  byte;
#endif

typedef uint16_t uint16;
typedef int16_t  int16;

typedef uint32_t uint32;
typedef int32_t  int32;

typedef uint64_t uint64;
typedef int64_t  int64;

// This enum is used for comparison functions. It should be compatible with
// standard C comparisons.
typedef enum comparison {
    COMPARE_LESS_THAN = -1,
    COMPARE_EQUAL_TO = 0,
    COMPARE_GREATER_THAN = 1,
} comparison;

#endif // DF_CORE_TYPES
