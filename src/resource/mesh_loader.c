#define LOG_CATEGORY "Resource"

#include "mesh_loader.h"

#include "container/array.h"
#include "check.h"
#include "data_loader.h"
#include "log/log.h"
#include "memory/alloc.h"
#include "mesh.h"
#include "paths.h"
#include <string.h>


typedef struct mesh_index {
    uint32 pos[3];
    uint32 uv[3];
    uint32 norm[3];
} mesh_index;

mesh load_mesh(const char* path) {
    const char* ext = get_extension(path);
    if(!strcmp(ext, "obj"))
        return load_obj_mesh(path);

    error("Failed to load mesh: File extension %s not recognized", ext);

    return NULL;
}

mesh load_obj_mesh(const char* path) {
    char* buf = (char*)load_data_buffer(path, NULL);
    check_return(buf, "Failed to load mesh", NULL);
    sarray position_list = sarray_new(0);
    sarray normal_list = sarray_new(0);
    sarray uv_list = sarray_new(0);
    sarray index_list = sarray_new(0);
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

                mesh_index* in = salloc(sizeof(mesh_index));
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
                array_add(index_list, in);
            } break;
            case 'v':
                switch(c[1]) {
                    case ' ': {
                                vec3* p = salloc(sizeof(vec3));
                                int count = sscanf(c, "v %f %f %f", &p->x, &p->y, &p->z);
                                check_warn(count == 3, "Malformed mesh %s: Expected 3 position coords in mesh, got %d", path, count);
                                array_add(position_list, p);
                            } break;
                    case 'p': break; // Ignore Parameter space verts
                    case 't': {
                                vec2* t = salloc(sizeof(vec2));
                                int count = sscanf(c, "vt %f %f", &t->x, &t->y);
                                check_warn(count == 2, "Malformed mesh %s: Expected 2 uv coords in mesh, got %d", path, count);
                                array_add(uv_list, t);
                            } break;
                    case 'n': {
                                vec3* n = salloc(sizeof(vec3));
                                int count = sscanf(c, "vn %f %f %f", &n->data[0], &n->data[1], &n->data[2]);
                                check_warn(count == 3, "Malformed mesh %s: Expected 3 normal coords in mesh, got %d", path, count);
                                array_add(normal_list, n);
                            } break;
                } break;
        }
        c = strchr(c, '\n');
        if(c[0] == '\n')
            ++c;
    }
    uint8 type = VT_POSITION;
    if(array_size(normal_list) > 0)
        type |= VT_NORMAL;
    if(array_size(uv_list) > 0)
        type |= VT_TEXTURE;
    mesh m = mesh_new_type(array_size(index_list) * 3, type, NULL);
    if(type & VT_NORMAL && type & VT_TEXTURE) {
        vt_pnt* data = (vt_pnt*)mesh_get_data(m);
        for(int i = 0; array_size(index_list) > 0; i += 3) {
            struct mesh_index* in = array_pop(index_list);
            vec3* p1 = array_get(position_list, in->pos[0]-1);
            vec3* n1 = array_get(normal_list, in->norm[0]-1);
            vec2* t1 = array_get(uv_list, in->uv[0]-1);
            vec3* p2 = array_get(position_list, in->pos[1]-1);
            vec3* n2 = array_get(normal_list, in->norm[1]-1);
            vec2* t2 = array_get(uv_list, in->uv[1]-1);
            vec3* p3 = array_get(position_list, in->pos[2]-1);
            vec3* n3 = array_get(normal_list, in->norm[2]-1);
            vec2* t3 = array_get(uv_list, in->uv[2]-1);

            data[i]   = (vt_pnt){ .position=*p1, .uv=*t1, .normal=*n1 };
            data[i+1] = (vt_pnt){ .position=*p2, .uv=*t2, .normal=*n2 };
            data[i+2] = (vt_pnt){ .position=*p3, .uv=*t3, .normal=*n3 };
        }
    } else if(type & VT_NORMAL) {
        vt_pn* data = (vt_pn*)mesh_get_data(m);
        for(int i = 0; array_size(index_list) > 0; i += 3) {
            struct mesh_index* in = array_pop(index_list);
            vec3* p1 = array_get(position_list, in->pos[0]-1);
            vec3* n1 = array_get(normal_list, in->norm[0]-1);
            vec3* p2 = array_get(position_list, in->pos[1]-1);
            vec3* n2 = array_get(normal_list, in->norm[1]-1);
            vec3* p3 = array_get(position_list, in->pos[2]-1);
            vec3* n3 = array_get(normal_list, in->norm[2]-1);

            data[i]   = (vt_pn){ .position=*p1, .normal=*n1 };
            data[i+1] = (vt_pn){ .position=*p2, .normal=*n2 };
            data[i+2] = (vt_pn){ .position=*p3, .normal=*n3 };
        }
    } else if(type & VT_TEXTURE) {
        vt_pt* data = (vt_pt*)mesh_get_data(m);
        for(int i = 0; array_size(index_list) > 0; i += 3) {
            struct mesh_index* in = array_pop(index_list);
            vec3* p1 = array_get(position_list, in->pos[0]-1);
            vec2* t1 = array_get(uv_list, in->uv[0]-1);
            vec3* p2 = array_get(position_list, in->pos[1]-1);
            vec2* t2 = array_get(uv_list, in->uv[1]-1);
            vec3* p3 = array_get(position_list, in->pos[2]-1);
            vec2* t3 = array_get(uv_list, in->uv[2]-1);

            data[i]   = (vt_pt){ .position=*p1, .uv=*t1 };
            data[i+1] = (vt_pt){ .position=*p2, .uv=*t2 };
            data[i+2] = (vt_pt){ .position=*p3, .uv=*t3 };
        }
    } else {
        vt_p* data = (vt_p*)mesh_get_data(m);
        for(int i = 0; array_size(index_list) > 0; i += 3) {
            struct mesh_index* in = array_pop(index_list);
            vec3* p1 = array_get(position_list, in->pos[0]-1);
            vec3* p2 = array_get(position_list, in->pos[1]-1);
            vec3* p3 = array_get(position_list, in->pos[2]-1);

            data[i]   = (vt_p){ .position=*p1 };
            data[i+1] = (vt_p){ .position=*p2 };
            data[i+2] = (vt_p){ .position=*p3 };
        }
    }

    array_free_deep(index_list);
    array_free_deep(position_list);
    array_free_deep(normal_list);
    array_free_deep(uv_list);
    sfree(buf);
    mesh_update(m);
    return m;
}
