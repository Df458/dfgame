#ifndef DF_MATH_UTIL
#define DF_MATH_UTIL

// Ensures that v is between the range of low and hi.
#define clamp(v, low, hi) (v > low ? (v > hi ? hi : v) : low)

// Returns the lesser value of a and b
#define min(a, b) (a < b ? a : b)

// Returns the lesser value of a and b
#define max(a, b) (a > b ? a : b)

#define compare(a, b) (a < b ? COMPARE_LESS_THAN : a == b ? COMPARE_EQUAL_TO : COMPARE_GREATER_THAN)

// pow() is slower than normal multiplication. so we can use these for low
// powers.
#define square(v) (v * v)
#define cube(v) (v * v * v)

// Multiplies theta by the correct constant to change degrees to radians and
// vice-versa.
#define degtorad(theta) (theta * 0.01745329251)
#define radtodeg(theta) (theta * 57.2957795131)

#define eq0(f) (f < 1e-6)

#define PI 3.14159265358979323846

#endif
