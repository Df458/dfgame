// Log category, used to filter logs
#define LOG_CATEGORY "Core"

#include "power.h"

#include <assert.h>

// Returns the next power of two greater than val
uint8 next_power_of_two_8(uint8 val) {
    assert(val > 0);
    assert(val <= UINT8_MAX / 2);

    // __builtin_clzll counts the leading zeroes in a number (gcc)
    return 1 << (8 - __builtin_clz(val));
}
uint16 next_power_of_two_16(uint16 val) {
    assert(val > 0);
    assert(val <= UINT16_MAX / 2);

    // __builtin_clzll counts the leading zeroes in a number (gcc)
    return 1 << (16 - __builtin_clz(val));
}
uint32 next_power_of_two_32(uint32 val) {
    assert(val > 0);
    assert(val <= UINT32_MAX / 2);

    // __builtin_clzll counts the leading zeroes in a number (gcc)
    return 1 << (32 - __builtin_clzl(val));
}
uint64 next_power_of_two_64(uint64 val) {
    assert(val > 0);
    assert(val <= UINT64_MAX / 2);

    // __builtin_clzll counts the leading zeroes in a number (gcc)
    return 1 << (64 - __builtin_clzll(val));
}

#undef LOG_CATEGORY
