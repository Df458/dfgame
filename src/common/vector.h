#ifndef DF_VECTOR_H
#define DF_VECTOR_H

// TODO: 3D vector support

typedef struct vec2 { float data[2]; } vec2;

vec2* create_vec2_data(float x, float y);
#define create_vec2() create_vec2_data(0.0f, 0.0f)
void destroy_vec2_full(vec2* vec);
#define destroy_vec2(vec) { destroy_vec2_full(vec); vec = 0; }

#endif
