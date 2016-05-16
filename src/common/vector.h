#ifndef DF_VECTOR_H
#define DF_VECTOR_H

// TODO: 3D vector support

typedef struct vec2 { float data[2]; } vec2;
typedef struct vec3 { float data[3]; } vec3;
typedef struct vec4 { float data[4]; } vec4;

vec2 create_vec2_data(float x, float y);
#define create_vec2() create_vec2_data(0.0f, 0.0f)
// void destroy_vec2_full(vec2* vec);
// #define destroy_vec2(vec) { destroy_vec2_full(vec); vec = 0; }

vec3 create_vec3_data(float x, float y, float z);
#define create_vec3() create_vec3_data(0.0f, 0.0f, 0.0f)
// void destroy_vec3_full(vec3* vec);
// #define destroy_vec3(vec) { destroy_vec3_full(vec); vec = 0; }

vec4 create_vec4_data(float x, float y, float z, float w);
#define create_vec4() create_vec4_data(0.0f, 0.0f, 0.0f, 0.0f)
// void destroy_vec4_full(vec4* vec);
// #define destroy_vec4(vec) { destroy_vec4_full(vec); vec = 0; }

#endif
