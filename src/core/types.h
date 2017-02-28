#ifndef DF_CORE_TYPES
#define DF_CORE_TYPES

#include <inttypes.h>
#include <stdbool.h>

#define INVALID_INDEX -1

#define declare(type, name) typedef type name name;
#define declarep(type, name) typedef type name* name;
#define delegate(type, name, ...) typedef type (*name)(__VA_ARGS__);

typedef uint8_t uint8;
typedef uint8_t ubyte;
typedef int8_t  int8;
typedef int8_t  byte;

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
    COMPARE_EQUAL_TO,
    COMPARE_GREATER_THAN,
} comparison;

#endif // DF_CORE_TYPES
