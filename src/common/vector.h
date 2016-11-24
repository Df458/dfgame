#ifndef DF_VECTOR_H
#define DF_VECTOR_H

typedef struct vec2 { float data[2]; } vec2;
typedef struct vec3 { float data[3]; } vec3;
typedef struct vec4 { float data[4]; } vec4;

vec2 create_vec2_data(float x, float y);
float vec2_dot(vec2 a, vec2 b);
#define create_vec2() create_vec2_data(0.0f, 0.0f)

vec3 create_vec3_data(float x, float y, float z);
float vec3_dot(vec3 a, vec3 b);
#define create_vec3() create_vec3_data(0.0f, 0.0f, 0.0f)

vec4 create_vec4_data(float x, float y, float z, float w);
float vec4_dot(vec4 a, vec4 b);
#define create_vec4() create_vec4_data(0.0f, 0.0f, 0.0f, 0.0f)

#endif
