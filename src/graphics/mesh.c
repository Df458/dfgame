// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "mesh.h"

#include "check.h"
#include "memory/alloc.h"
#include "stringutil.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

// Represents a mesh
typedef struct mesh {
    void*    data;
    uint8 type_flags;
    uint32 vertex_count;
    uint32 data_size;
    GLuint handle;

    char* asset_path;
}* mesh;

// Creates a new mesh struct.
// setting data to NULL creates an empty mesh struct
mesh mesh_new_type(uint32_t size, uint8_t vertex_type, void* data, const char* path) {
    check_return(vertex_type & VT_POSITION, "Unsupported vertex type %u: Type must contain position.", NULL, vertex_type);

    mesh m = salloc(sizeof(struct mesh));
    m->data = NULL;
    m->asset_path = nstrdup(path);
    glGenBuffers(1, &m->handle);

    mesh_set_data_type(m, size, vertex_type, data);

    return m;
}

void _mesh_render(shader s, mesh m, GLenum mode, ...) {
    check_return(m != NULL, "Mesh is NULL", );

    va_list args;
    va_start(args, mode);
    shader_bind_attribute_mesh_va(s, m, args);
    va_end(args);
    glDrawArrays(mode, 0, m->vertex_count);
}

// Frees the mesh. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _mesh_free(mesh m) {
    if(!m)
        return;
    glDeleteBuffers(1, &m->handle);
    sfree(m->data);
    if(m->asset_path)
        sfree(m->asset_path);
    sfree(m);
}

// Returns the OpenGL handle for the mesh's data.
GLuint mesh_get_handle(mesh m) {
    check_return(m, "Mesh is NULL", 0);

    return m->handle;
}

// Returns whether or not the mesh's data has the vertex data described in type.
bool mesh_has_vertex_data(mesh m, vertex_types type) {
    check_return(m != NULL, "Mesh is NULL", false);

    return m->type_flags & type;
}

// Returns the size of a vertex in the mesh, in bytes.
uint32 mesh_get_data_size(mesh m) {
    check_return(m != NULL, "Mesh is NULL", 0);

    return m->data_size;
}

// Returns the type of a vertex in the mesh, in bytes.
uint8 mesh_get_data_type(mesh m) {
    check_return(m != NULL, "Mesh is NULL", 0);

    return m->type_flags;
}

// Returns the contents of the mesh.
void* mesh_get_data(mesh m) {
    check_return(m != NULL, "Mesh is NULL", NULL);

    return m->data;
}
//
// Returns the number of vertices in the mesh
uint32 mesh_get_vertex_count(mesh m) {
    check_return(m != NULL, "Mesh is NULL", 0);

    return m->vertex_count;
}

void mesh_set_data_type(mesh m, uint32 size, uint8 vertex_type, void* data) {
    check_return(m != NULL, "Mesh is NULL", );
    check_return(vertex_type & VT_POSITION, "Unsupported vertex type %u: Type must contain position.", , vertex_type);

    if(m->data)
        sfree(m->data);

    uint32 vt_size = sizeof(float) * 3;
    if(vertex_type & VT_NORMAL)
        vt_size += sizeof(float) * 3;
    if(vertex_type & VT_TEXTURE)
        vt_size += sizeof(float) * 2;
    if(vertex_type & VT_COLOR)
        vt_size += sizeof(float) * 4;

    m->data = scalloc(size, vt_size);

    if(data)
        memcpy(m->data, data, size * vt_size);

    m->type_flags = vertex_type;
    m->vertex_count = size;
    m->data_size = vt_size;

    mesh_update(m);
}

// Updates the mesh for rendering
void mesh_update(mesh m) {
    check_return(m != NULL, "Mesh is NULL", );

    glBindBuffer(GL_ARRAY_BUFFER, m->handle);
    glBufferData(GL_ARRAY_BUFFER, m->vertex_count * m->data_size, m->data, GL_DYNAMIC_DRAW);
}

// Returns the offset in bytes of the vertex data described in type.
// Return value is an integer typed as GLvoid* in order to satisfy the
// requirements of glVertexAttribPointer.
GLvoid* mesh_get_element_offset(mesh m, vertex_types type) {
    check_return(m != NULL, "Mesh is NULL", NULL);

    GLvoid* count = 0;
    if(type == VT_POSITION)
        return count;
    if(m->type_flags & VT_POSITION)
        count += 3 * sizeof(float);

    if(type == VT_NORMAL)
        return count;
    if(m->type_flags & VT_NORMAL)
        count += 3 * sizeof(float);

    if(type == VT_TEXTURE)
        return count;
    if(m->type_flags & VT_TEXTURE)
        count += 2 * sizeof(float);

    if(type == VT_COLOR)
        return count;

    return 0;
}

const vt_pnt mesh_quad_verts[] = {
    (vt_pnt){.data={-0.5f, -0.5f, 0,  0, 0, 1,  0, 0}},
    (vt_pnt){.data={ 0.5f, -0.5f, 0,  0, 0, 1,  1, 0}},
    (vt_pnt){.data={ 0.5f,  0.5f, 0,  0, 0, 1,  1, 1}},
                                                    
    (vt_pnt){.data={-0.5f, -0.5f, 0,  0, 0, 1,  0, 0}},
    (vt_pnt){.data={ 0.5f,  0.5f, 0,  0, 0, 1,  1, 1}},
    (vt_pnt){.data={-0.5f,  0.5f, 0,  0, 0, 1,  0, 1}},
};

static struct mesh mesh_quad_data = (struct mesh) {
    .data = (void*)mesh_quad_verts,
    .type_flags = VT_POSITION | VT_NORMAL | VT_TEXTURE,
    .vertex_count = 6,
    .data_size = sizeof(vt_pnt),
    .handle = -1
};

mesh mesh_quad() {
    if(mesh_quad_data.handle == -1) {
        glGenBuffers(1, &mesh_quad_data.handle);
        mesh_update(&mesh_quad_data);
    }
    return &mesh_quad_data;
}
