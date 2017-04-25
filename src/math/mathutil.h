#ifndef DF_MATH_UTIL
#define DF_MATH_UTIL

// Ensures that v is between the range of low and hi.
#define clamp(v, low, hi) (v > low ? (v > hi ? hi : v) : low)

// pow() is slower than normal multiplication. so we can use these for low
// powers.
#define square(v) (v * v)
#define cube(v) (v * v * v)

// Multiplies theta by the correct constant to change degrees to radians and
// vice-versa.
#define degtorad(theta) (theta * 0.01745329251)
#define radtodeg(theta) (theta * 57.2957795131)

#define PI 3.14159265358979323846

#endif
