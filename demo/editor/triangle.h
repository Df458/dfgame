#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "mesh.h"
#include "vector.h"

typedef struct triangle {
    float size;
    float angle;
    vec4  color;
    bool  filled;

    mesh tri_data;
} triangle;

void init();
void cleanup();

triangle* triangle_init();
void triangle_free(triangle* t);
void triangle_draw(triangle* t);
void triangle_load(triangle* t, char* data);
char* triangle_save(triangle* t);

#endif
