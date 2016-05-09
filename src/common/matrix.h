#ifndef DF_MATRIX_H
#define DF_MATRIX_H
#include <inttypes.h>

typedef struct mat4 { float data[16]; } mat4;

#define ident { {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1} }

mat4 create_mat4();
void mat4_ortho(mat4* mat, float left, float right, float bottom, float top, float near, float far);
mat4 mat4_mul(mat4 lv, mat4 rv);

// NOTE: These transforms are 2D
void mat4_translate(mat4* mat, float position_x, float position_y, uint8_t relative);
void mat4_rotate(mat4* mat, float angle, uint8_t relative);
void mat4_scale(mat4* mat, float scale_x, float scale_y, uint8_t relative);
void mat4_reset(mat4* mat);
void mat4_invert(mat4* mat);
float mat4_get_determinant(mat4 mat);
void mat4_transpose(mat4* mat);

#endif
