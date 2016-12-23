#ifndef DF_VECTOR_H
#define DF_VECTOR_H

typedef struct vec2 {
    union {
        struct {
            float x;
            float y;
        };
        float data[2];
    };
} vec2;
typedef struct vec3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float data[3];
    };
} vec3;
typedef struct vec4 {
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        float data[4];
    };
} vec4;

float vec2_dot(vec2 a, vec2 b);

float vec3_dot(vec3 a, vec3 b);

float vec4_dot(vec4 a, vec4 b);

#endif
