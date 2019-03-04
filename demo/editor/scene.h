#ifndef SCENE_H
#define SCENE_H
#include "core/container/array.h"

#include <libxml/tree.h>

typedef struct scene {
    array tris;
} scene;

scene* scene_init();
void scene_free(scene* s);
void scene_draw(scene* s);
void scene_load(scene* s, xmlNodePtr data);
void scene_reset(scene* s);
char* scene_save(scene* s);

#endif
