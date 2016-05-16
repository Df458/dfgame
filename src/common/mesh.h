#ifndef DF_MESH_H
#define DF_MESH_H
#include "resource_def.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct mesh
{
    void*    data;
    uint8_t  type_flags;
    uint32_t vertex_count;
    GLuint handle;
}
mesh;

mesh* create_mesh(uint32_t size, uint8_t vertex_type);
void destroy_mesh_full(mesh* m);
#define destroy_mesh(m) destroy_mesh_full(m); m = 0;
mesh* load_resource_to_mesh(resource_pair);
bool save_mesh_to_resource(resource_pair);

#endif
