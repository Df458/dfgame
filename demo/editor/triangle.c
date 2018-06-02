#include "check.h"
#include "color.h"
#include "input.h"
#include "log/log.h"
#include "matrix.h"
#include "graphics_log.h"
#include "shader_init.h"
#include "triangle.h"
#include "vertex.hd"
#include <GL/glew.h>
#include <GL/gl.h>

shader s_main;

vt_p triangle_verts[3] = {
    (vt_p){.data={0, -1, 0}},
    (vt_p){.data={1, 1, 0}},
    (vt_p){.data={-1, 1, 0}},
};

void boop(action_id id, void* user) {
    info("?????");
}

void init() {
    glewExperimental = 1;
    check_kill(glewInit() == GLEW_OK, "Failed to initialize GLEW");
    if(!check_error(glewIsExtensionSupported("GL_ARB_debug_output"), "Graphics debug logging is not available for your hardware. Graphical errors will not be reported")) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(graphics_log, NULL);

        GLuint VAO; // Setup default VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaders_init();

    s_main = shader_basic_untex_get();

    input_add_mouse_button_action(MB_LEFT, as_event(action_event, boop, NULL));
}

void cleanup() {
    shaders_cleanup();
}

triangle* triangle_init() {
    triangle* t = msalloc(struct triangle);

    t->size  = 0.5f;
    t->angle = 0.0f;
    t->color = color_white;
    t->filled = true;
    t->tri_data = mesh_new(3, triangle_verts, NULL);

    return t;
}
void triangle_free(triangle* t) {
    sfree(t);
}
void triangle_draw(triangle* t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(s_main.id);
    shader_bind_uniform_name(s_main, "transform", mat4_rotate_2d(mat4_scale(mat4_ident, t->size), degtorad(t->angle + 180)));
    shader_bind_uniform_name(s_main, "u_color", t->color);
    glPolygonMode(GL_FRONT_AND_BACK, t->filled ? GL_FILL : GL_LINE);
    mesh_render(s_main, t->tri_data, GL_TRIANGLES, "i_pos", VT_POSITION);
}
