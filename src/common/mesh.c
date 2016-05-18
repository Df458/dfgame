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
    m->data_size = vt_size;

    glGenBuffers(1, &m->handle);
    mesh_update(m);
    checkGLError();

    return m;
}

void destroy_mesh_full(mesh* m)
{
    glDeleteBuffers(1, &m->handle);
    sfree(m->data);
    sfree(m);
}

struct mesh_index
{
    uint32_t pos[3];
    uint32_t uv[3];
    uint32_t norm[3];
};

mesh* load_resource_to_mesh(resource_pair)
{
    char* buf = load_resource_to_buffer(resource_location, resource_name);
    array_list* position_list = create_array_list();
    array_list* normal_list = create_array_list();
    array_list* uv_list = create_array_list();
    array_list* index_list = create_array_list();
    char* c = buf;
    while(c[0] != 0) {
        switch(c[0]) {
            case '#': break; // Ignore comments
            case 'm': case 'u': break; // Ignore materials
            // TODO: Add multi-object loading support
            case 'o': break; // Ignore object definitions
            case 's': break; // Ignore smoothing
            case 'f': {
                // Determine the format of the line
                // TODO: Detect and fix quad meshes
                bool has_normals = false;
                bool has_uvs = false;
                if(c[1] != 0 && c[1] != '\n') {
                    for(int i = 2; c[i] != 0 && c[i] != '\n' && c[i] != ' '; ++i) {
                        if(c[i] == '/') {
                            if(has_uvs || c[i+1] == '/') {
                                has_normals = true;
                                break;
                            } else {
                                has_uvs = true;
                            }
                        }
                    }
                }

                struct mesh_index* in = salloc(sizeof(struct mesh_index));
                // Load the index data
                if(has_uvs) {
                    if(has_normals)
                        sscanf(c, "\nf %u/%u/%u %u/%u/%u %u/%u/%u", &in->pos[0], &in->uv[0], &in->norm[0], &in->pos[1], &in->uv[1], &in->norm[1], &in->pos[2], &in->uv[2], &in->norm[2]);
                    else
                        sscanf(c, "\nf %u/%u %u/%u %u/%u", &in->pos[0], &in->uv[0], &in->pos[1], &in->uv[1], &in->pos[2], &in->uv[2]);
                } else if(has_normals) {
                    sscanf(c, "\nf %u//%u %u//%u %u//%u", &in->pos[0], &in->norm[0], &in->pos[1], &in->norm[1], &in->pos[2], &in->norm[2]);
                } else
                    sscanf(c, "\nf %u %u %u", &in->pos[0], &in->pos[1], &in->pos[2]);
                array_list_push(index_list, in);
            } break;
            case 'v':
                switch(c[1]) {
                    case ' ': {
                        vec3* p = salloc(sizeof(vec3));
                        *p = create_vec3();
                        int count = sscanf(c, "v %f %f %f", &p->data[0], &p->data[1], &p->data[2]);
                        if(count != 3)
                            error("Malformed mesh %s: Expected 3 position coords in mesh, got %d", resource_name, count);
                        array_list_push(position_list, p);
                    } break;
                    case 'p': break; // Ignore Parameter space verts
                    case 't': {
                        vec2* t = salloc(sizeof(vec2));
                        *t = create_vec2();
                        int count = sscanf(c, "vt %f %f", &t->data[0], &t->data[1]);
                        if(count != 2)
                            error("Malformed mesh %s: Expected 2 uv coords in mesh, got %d", resource_name, count);
                        array_list_push(uv_list, t);
                    } break;
                    case 'n': {
                        vec3* n = salloc(sizeof(vec3));
                        *n = create_vec3();
                        int count = sscanf(c, "vn %f %f %f", &n->data[0], &n->data[1], &n->data[2]);
                        if(count != 3)
                            error("Malformed mesh %s: Expected 3 normal coords in mesh, got %d", resource_name, count);
                        array_list_push(normal_list, n);
                    } break;
                }
            break;
        }
        c = strchr(c, '\n');
        if(c[0] == '\n')
            ++c;
    }
    uint8_t type = VT_POSITION;
    if(normal_list->length > 0)
        type |= VT_NORMAL;
    if(uv_list->length > 0)
        type |= VT_TEXTURE;
    mesh* m = create_mesh(index_list->length * 3, type);
    if(type & VT_NORMAL && type & VT_TEXTURE) {
        vt_pnt* data = m->data;
        for(int i = 0; index_list->length > 0; i += 3) {
            struct mesh_index* in = array_list_pop(index_list);
            vec3* p1 = array_list_get(position_list, in->pos[0]-1);
            vec3* n1 = array_list_get(normal_list, in->norm[0]-1);
            vec2* t1 = array_list_get(uv_list, in->uv[0]-1);
            vec3* p2 = array_list_get(position_list, in->pos[1]-1);
            vec3* n2 = array_list_get(normal_list, in->norm[1]-1);
            vec2* t2 = array_list_get(uv_list, in->uv[1]-1);
            vec3* p3 = array_list_get(position_list, in->pos[2]-1);
            vec3* n3 = array_list_get(normal_list, in->norm[2]-1);
            vec2* t3 = array_list_get(uv_list, in->uv[2]-1);
            data[i].position[0] = p1->data[0];
            data[i].position[1] = p1->data[1];
            data[i].position[2] = p1->data[2];
            data[i].normal[0] = n1->data[0];
            data[i].normal[1] = n1->data[1];
            data[i].normal[2] = n1->data[2];
            data[i].texture[0] = t1->data[0];
            data[i].texture[1] = t1->data[1];
            data[i].texture[2] = t1->data[2];
            data[i+1].position[0] = p2->data[0];
            data[i+1].position[1] = p2->data[1];
            data[i+1].position[2] = p2->data[2];
            data[i+1].normal[0] = n2->data[0];
            data[i+1].normal[1] = n2->data[1];
            data[i+1].normal[2] = n2->data[2];
            data[i+1].texture[0] = t2->data[0];
            data[i+1].texture[1] = t2->data[1];
            data[i+1].texture[2] = t2->data[2];
            data[i+2].position[0] = p3->data[0];
            data[i+2].position[1] = p3->data[1];
            data[i+2].position[2] = p3->data[2];
            data[i+2].normal[0] = n3->data[0];
            data[i+2].normal[1] = n2->data[1];
            data[i+2].normal[2] = n3->data[2];
            data[i+2].texture[0] = t3->data[0];
            data[i+2].texture[1] = t3->data[1];
            data[i+2].texture[2] = t3->data[2];
        }
    } else if(type & VT_NORMAL) {
        vt_pn* data = m->data;
        for(int i = 0; index_list->length > 0; i += 3) {
            struct mesh_index* in = array_list_pop(index_list);
            vec3* p1 = array_list_get(position_list, in->pos[0]-1);
            vec3* n1 = array_list_get(normal_list, in->norm[0]-1);
            vec3* p2 = array_list_get(position_list, in->pos[1]-1);
            vec3* n2 = array_list_get(normal_list, in->norm[1]-1);
            vec3* p3 = array_list_get(position_list, in->pos[2]-1);
            vec3* n3 = array_list_get(normal_list, in->norm[2]-1);
            data[i].position[0] = p1->data[0];
            data[i].position[1] = p1->data[1];
            data[i].position[2] = p1->data[2];
            data[i].normal[0] = n1->data[0];
            data[i].normal[1] = n1->data[1];
            data[i].normal[2] = n1->data[2];
            data[i+1].position[0] = p2->data[0];
            data[i+1].position[1] = p2->data[1];
            data[i+1].position[2] = p2->data[2];
            data[i+1].normal[0] = n2->data[0];
            data[i+1].normal[1] = n2->data[1];
            data[i+1].normal[2] = n2->data[2];
            data[i+2].position[0] = p3->data[0];
            data[i+2].position[1] = p3->data[1];
            data[i+2].position[2] = p3->data[2];
            data[i+2].normal[0] = n3->data[0];
            data[i+2].normal[1] = n3->data[1];
            data[i+2].normal[2] = n3->data[2];
        }
    } else if(type & VT_TEXTURE) {
        vt_pt* data = m->data;
        for(int i = 0; index_list->length > 0; i += 3) {
            struct mesh_index* in = array_list_pop(index_list);
            vec3* p1 = array_list_get(position_list, in->pos[0]-1);
            vec2* t1 = array_list_get(uv_list, in->uv[0]-1);
            vec3* p2 = array_list_get(position_list, in->pos[1]-1);
            vec2* t2 = array_list_get(uv_list, in->uv[1]-1);
            vec3* p3 = array_list_get(position_list, in->pos[2]-1);
            vec2* t3 = array_list_get(uv_list, in->uv[2]-1);
            data[i].position[0] = p1->data[0];
            data[i].position[1] = p1->data[1];
            data[i].position[2] = p1->data[2];
            data[i].texture[0] = t1->data[0];
            data[i].texture[1] = t1->data[1];
            data[i].texture[2] = t1->data[2];
            data[i+1].position[0] = p2->data[0];
            data[i+1].position[1] = p2->data[1];
            data[i+1].position[2] = p2->data[2];
            data[i+1].texture[0] = t2->data[0];
            data[i+1].texture[1] = t2->data[1];
            data[i+1].texture[2] = t2->data[2];
            data[i+2].position[0] = p3->data[0];
            data[i+2].position[1] = p3->data[1];
            data[i+2].position[2] = p3->data[2];
            data[i+2].texture[0] = t3->data[0];
            data[i+2].texture[1] = t3->data[1];
            data[i+2].texture[2] = t3->data[2];
        }
    } else {
        vt_p* data = m->data;
        for(int i = 0; index_list->length > 0; i += 3) {
            struct mesh_index* in = array_list_pop(index_list);
            vec3* p1 = array_list_get(position_list, in->pos[0]-1);
            vec3* p2 = array_list_get(position_list, in->pos[1]-1);
            vec3* p3 = array_list_get(position_list, in->pos[2]-1);
            data[i].position[0] = p1->data[0];
            data[i].position[1] = p1->data[1];
            data[i].position[2] = p1->data[2];
            data[i+1].position[0] = p2->data[0];
            data[i+1].position[1] = p2->data[1];
            data[i+1].position[2] = p2->data[2];
            data[i+2].position[0] = p3->data[0];
            data[i+2].position[1] = p3->data[1];
            data[i+2].position[2] = p3->data[2];
        }
    }
    array_list_destroy_deep(index_list);
    array_list_destroy_deep(position_list);
    array_list_destroy_deep(normal_list);
    array_list_destroy_deep(uv_list);
    sfree(buf);
    mesh_update(m);
    return m;
}

bool save_mesh_to_resource(resource_pair)
{
    stub(false);
}

void mesh_update(mesh* m)
{
    glBindBuffer(GL_ARRAY_BUFFER, m->handle);
    glBufferData(GL_ARRAY_BUFFER, m->vertex_count * m->data_size, m->data, GL_DYNAMIC_DRAW);
    checkGLError();
}
