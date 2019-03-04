#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "mesh.h"
#include "vector.h"

#include <libxml/tree.h>
#include <libxml/xmlwriter.h>

typedef struct triangle {
    float size;
    float angle;
    vec4  color;
    bool  filled;
    int line_width;

    mesh tri_data;
} triangle;

void init();
void cleanup();

triangle* triangle_init();
void triangle_free(triangle* t);
void triangle_draw(triangle* t);
void triangle_load(triangle* t, xmlNodePtr data);
void triangle_save(triangle* t, xmlTextWriter* writer);

#endif
