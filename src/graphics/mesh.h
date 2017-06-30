#ifndef DF_GRAPHICS_MESH
#define DF_GRAPHICS_MESH
#include "mesh.hd"
#include "vertex.hd"
#include <GL/glew.h>
#include <GL/gl.h>

// Creates a new mesh struct.
// setting data to NULL creates an empty mesh struct
mesh mesh_new_type(uint32 size, uint8 vertex_type, void* data);

#define mesh_new(size, data) _Generic(data,\
    vt_p*: mesh_new_type(size, VT_POSITION, data),\
    vt_pn*: mesh_new_type(size, VT_POSITION | VT_NORMAL, data),\
    vt_pt*: mesh_new_type(size, VT_POSITION | VT_TEXTURE, data),\
    vt_pnt*: mesh_new_type(size, VT_POSITION | VT_NORMAL | VT_TEXTURE, data),\
    vt_pc*: mesh_new_type(size, VT_POSITION | VT_COLOR, data),\
    vt_pnc*: mesh_new_type(size, VT_POSITION | VT_NORMAL | VT_COLOR, data),\
    vt_ptc*: mesh_new_type(size, VT_POSITION | VT_TEXTURE | VT_COLOR, data),\
    vt_pntc*: mesh_new_type(size, VT_POSITION | VT_NORMAL | VT_TEXTURE | VT_COLOR, data)\
)

void mesh_render(mesh m, GLenum mode);

// Frees the mesh and sets it to NULL to make it harder to double-free.
#define mesh_free(m) { _mesh_free(m); m = NULL; }

// Frees the mesh. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _mesh_free(mesh m);

// Returns the OpenGL handle for the mesh's data.
GLuint mesh_get_handle(mesh m);

// Returns whether or not the mesh's data has the vertex data described in type.
bool mesh_has_vertex_data(mesh m, vertex_types type);

// Returns the size of a vertex in the mesh, in bytes.
uint32 mesh_get_data_size(mesh m);

// Returns the type of a vertex in the mesh, in bytes.
uint8 mesh_get_data_type(mesh m);

// Returns the contents of the mesh.
void* mesh_get_data(mesh m);

// Updates the mesh for rendering
void mesh_update(mesh m);

// Returns the offset in bytes of the vertex data described in type.
// Return value is an integer typed as GLvoid* in order to satisfy the
// requirements of glVertexAttribPointer.
GLvoid* mesh_get_element_offset(mesh m, vertex_types type);

mesh mesh_quad();

#endif
