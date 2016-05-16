#include "array_list.h"
#include "io_util.h"
#include "mesh.h"
#include "render.h"
#include "util.h"
#include "vertex_def.h"

mesh* create_mesh(uint32_t size, uint8_t vertex_type)
{
    if(!(vertex_type & VT_POSITION)) {
        error("Unsupported vertex type %u: Type must contain position.");
        return 0;
    }
    uint32_t vt_size = sizeof(float) * 3;
    if(vertex_type & VT_NORMAL)
        vt_size += sizeof(float) * 3;
    if(vertex_type & VT_TEXTURE)
        vt_size += sizeof(float) * 2;
    if(vertex_type & VT_COLOR)
        vt_size += sizeof(GLubyte) * 4;
    mesh* m = salloc(sizeof(mesh));

    m->data = scalloc(size, vt_size);
    m->type_flags = vertex_type;
    m->vertex_count = size;

    glGenBuffers(1, &m->handle);
    glBindBuffer(GL_ARRAY_BUFFER, m->handle);
    glBufferData(GL_ARRAY_BUFFER, size * vt_size, m->data, GL_DYNAMIC_DRAW);
    checkGLError();

    return m;
}

void destroy_mesh_full(mesh* m)
{
    stub();
}

mesh* load_resource_to_mesh(resource_pair)
{
    char* buf = load_resource_to_buffer(resource_location, resource_name);
    array_list* position_list = create_array_list();
    array_list* normal_list = create_array_list();
    array_list* uv_list = create_array_list();
    // TODO: Populate lists
    for(int i = 0; i < strlen(buf); ++i) {
    }
    // TODO: Determine type/Acquire data
    // TODO: Build faces
    stub(0);
}

bool save_mesh_to_resource(resource_pair)
{
    stub(false);
}
