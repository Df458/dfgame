#ifndef DF_VERTEX_DEF
#define DF_VERTEX_DEF
#include <GL/glew.h>
#include <GL/gl.h>

enum vertex_types
{
    VT_POSITION = 0b10000000,
    VT_NORMAL = 0b01000000,
    VT_TEXTURE = 0b00100000,
    VT_COLOR = 0b00010000,
};

typedef struct vt_p
{
    GLfloat position[3];
}
vt_p;

typedef struct vt_pn
{
    GLfloat position[3];
    GLfloat normal[3];
}
vt_pn;

typedef struct vt_pt
{
    GLfloat position[3];
    GLfloat texture[2];
}
vt_pt;

typedef struct vt_pnt
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat texture[2];
}
vt_pnt;

typedef struct vt_pc
{
    GLfloat position[3];
    GLubyte color[4];
}
vt_pc;

typedef struct vt_pnc
{
    GLfloat position[3];
    GLfloat normal[3];
    GLubyte color[4];
}
vt_pnc;

typedef struct vt_ptc
{
    GLfloat position[3];
    GLfloat texture[2];
    GLubyte color[4];
}
vt_ptc;

typedef struct vt_pntc
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat texture[2];
    GLubyte color[4];
}
vt_pntc;

#endif
