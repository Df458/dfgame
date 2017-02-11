#ifndef DF_MATH_VECTOR
#define DF_MATH_VECTOR

// Represents a 2D vector
typedef struct vec2 {
    union {
        struct {
            float x;
            float y;
        };
        float data[2];
    };
} vec2;

// Represents a 3D vector
typedef struct vec3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        struct {
            float r;
            float g;
            float b;
        };
        float data[3];
        vec2 xy;
    };
} vec3;

// Represents a 4D vector
typedef struct vec4 {
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        struct {
            float r;
            float g;
            float b;
            float a;
        };
        float data[4];
        vec3 xyz;
        vec3 rgb;
        vec2 xy;
    };
} vec4;

// Returns the length of the vactor.
// NOTE: When comparing two vectors, vec_len_squared may be preferrable as it's
// much faster.
#define vec_len(v) _Generic(v,\
    vec2: vec2_len(v),\
    vec3: vec3_len(v),\
    vec4: vec4_len(v)\
)
float vec2_len(vec2 v);
float vec3_len(vec3 v);
float vec4_len(vec4 v);

// Returns the squared length of the vactor.
#define vec_len_squared(v) _Generic(v,\
    vec2: vec2_len_squared(v),\
    vec3: vec3_len_squared(v),\
    vec4: vec4_len_squared(v)\
)
float vec2_len_squared(vec2 v);
float vec3_len_squared(vec3 v);
float vec4_len_squared(vec4 v);

// Normalizes the vector, making its length 1.
#define vec_normalize(v) _Generic(v,\
    vec2: vec2_normalize(v),\
    vec3: vec3_normalize(v),\
    vec4: vec4_normalize(v)\
)
vec2 vec2_normalize(vec2 v);
vec3 vec3_normalize(vec3 v);
vec4 vec4_normalize(vec4 v);

// Adds 2 vectors together
#define vec_add(v1, v2) _Generic(v1,\
    vec2: vec2_add(v1, v2),\
    vec3: vec3_add(v1, v2),\
    vec4: vec4_add(v1, v2)\
)
vec2 vec2_add(vec2 v1, vec2 v2);
vec3 vec3_add(vec3 v1, vec3 v2);
vec4 vec4_add(vec4 v1, vec4 v2);

// Subtracts v2 from v1
#define vec_sub(v1, v2) _Generic(v1,\
    vec2: vec2_sub(v1, v2),\
    vec3: vec3_sub(v1, v2),\
    vec4: vec4_sub(v1, v2)\
)
vec2 vec2_sub(vec2 v1, vec2 v2);
vec3 vec3_sub(vec3 v1, vec3 v2);
vec4 vec4_sub(vec4 v1, vec4 v2);

// Multiplies v by the scalar x.
#define vec_mul(v, f) _Generic(v,\
    vec2: vec2_mul(v1, f),\
    vec3: vec3_mul(v1, f),\
    vec4: vec4_mul(v1, f)\
)
vec2 vec2_mul(vec2 v, float f);
vec3 vec3_mul(vec3 v, float f);
vec4 vec4_mul(vec4 v, float f);

// Returns the dot product between v1 and v2.
#define vec_dot(v1, v2) _Generic(v1,\
    vec2: vec2_dot(v1, v2),\
    vec3: vec3_dot(v1, v2),\
    vec4: vec4_dot(v1, v2)\
)
float vec2_dot(vec2 v1, vec2 v2);
float vec3_dot(vec3 v1, vec3 v2);
float vec4_dot(vec4 v1, vec4 v2);

// Returns the cross product of v1 and v2.
#define vec_cross(v1, v2) _Generic(v1,\
    vec3: vec3_cross(v1, v2),\
)
vec3 vec3_cross(vec3 v1, vec3 v2);

#endif
