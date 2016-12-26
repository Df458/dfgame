#ifndef DF_CORE_POWER
#define DF_CORE_POWER

#include "types.h"

// Returns the next power of two greater than val
#define next_power_of_two(val) _Generic(val,\
    uint8:  next_power_of_two_8(val),\
    uint16: next_power_of_two_16(val),\
    uint32: next_power_of_two_32(val),\
    uint64: next_power_of_two_64(val)\
)
uint8  next_power_of_two_8(uint8 val) __attribute__((const));
uint16 next_power_of_two_16(uint16 val) __attribute__((const));
uint32 next_power_of_two_32(uint32 val) __attribute__((const));
uint64 next_power_of_two_64(uint64 val) __attribute__((const));

#endif // DF_CORE_POWER
