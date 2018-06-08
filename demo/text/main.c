#include "core/types.h"
#include "core/log/log.h"
#include "application/game/window.h"
#include "application/game/mainloop.h"
#include "application/input.h"
#include "graphics/font.h"
#include "graphics/text.h"

#include "math/matrix.h"
#include "resource/paths.h"
#include "resource/font_loader.h"
#include "graphics/camera.h"
#include "graphics/color.h"
#include "graphics/debug_draw.h"
#include "graphics/shader.h"
#include "graphics/shader_init.h"
#include <math.h>
#include <GL/glew.h>
#include <GL/gl.h>

window win;

camera c_main;

text t_wrap_c;
text t_wrap_w;
text t_wrap_n;
text t_info;
font f_text;
shader s_text;
float counter = 0;

const char* demo_str = "Check it out, it's text wrapping!";
const char* info_str = "Q W E\nA S  D - Change text alignment\nZ X  C";

typedef struct align_key {
    key_id key;
    text_alignment alignment;
} align_key;

#define ALIGN_KEYS_COUNT 9
align_key keys[ALIGN_KEYS_COUNT] = {
    { .key = K_Q, .alignment = TEXT_ALIGN_TOP_LEFT     },
    { .key = K_W, .alignment = TEXT_ALIGN_TOP          },
    { .key = K_E, .alignment = TEXT_ALIGN_TOP_RIGHT    },
    { .key = K_A, .alignment = TEXT_ALIGN_LEFT         },
    { .key = K_S, .alignment = TEXT_ALIGN_CENTER       },
    { .key = K_D, .alignment = TEXT_ALIGN_RIGHT        },
    { .key = K_Z, .alignment = TEXT_ALIGN_BOTTOM_LEFT  },
    { .key = K_X, .alignment = TEXT_ALIGN_BOTTOM       },
    { .key = K_C, .alignment = TEXT_ALIGN_BOTTOM_RIGHT },
};

void wrap(action_id id, void* user) {
    text_set_align(t_wrap_c, (text_alignment)user);
    text_set_align(t_wrap_w, (text_alignment)user);
    text_set_align(t_wrap_n, (text_alignment)user);
}

void draw_lines(text_alignment align, vec2 pos, float level) {
    vt_pc a,b;
    a.position.xy = (vec2){ .x = 0, .y = pos.y - 100 };
    a.color = color_red;
    b.position.xy = (vec2){ .x = 0, .y = pos.y + 100 };
    b.color = color_red;

    switch(align) {
        case TEXT_ALIGN_TOP_RIGHT:
        case TEXT_ALIGN_RIGHT:
        case TEXT_ALIGN_BOTTOM_RIGHT:
            a.position.x = pos.x - level;
            b.position.x = pos.x - level;
            debug_draw_line(camera_get_vp(c_main), a, b, 2);
            break;
        case TEXT_ALIGN_TOP:
        case TEXT_ALIGN_CENTER:
        case TEXT_ALIGN_BOTTOM:
            a.position.x = pos.x - level * 0.5f;
            b.position.x = pos.x - level * 0.5f;
            debug_draw_line(camera_get_vp(c_main), a, b, 2);
            a.position.x = pos.x + level * 0.5f;
            b.position.x = pos.x + level * 0.5f;
            debug_draw_line(camera_get_vp(c_main), a, b, 2);
            break;
        case TEXT_ALIGN_TOP_LEFT:
        case TEXT_ALIGN_LEFT:
        case TEXT_ALIGN_BOTTOM_LEFT:
            a.position.x = pos.x + level;
            b.position.x = pos.x + level;
            debug_draw_line(camera_get_vp(c_main), a, b, 2);
            break;
    }
}

bool loop(mainloop l, float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    counter += dt;

    float level = sin(counter) * 60 + 61;
    text_set_max_width(t_wrap_c, level);
    text_set_max_width(t_wrap_w, level);

    glUseProgram(s_text.id);
    vec2 pos = (vec2){ .x=-320, .y=0 };
    text_draw(t_wrap_w, s_text, mat4_mul(camera_get_vp(c_main), mat4_translate(mat4_ident, pos)));
    draw_lines(text_get_align(t_wrap_w), pos, level);

    pos.x += 320;
    text_draw(t_wrap_c, s_text, mat4_mul(camera_get_vp(c_main), mat4_translate(mat4_ident, pos)));
    draw_lines(text_get_align(t_wrap_c), pos, level);

    pos.x += 320;
    text_draw(t_wrap_n, s_text, mat4_mul(camera_get_vp(c_main), mat4_translate(mat4_ident, pos)));
    draw_lines(text_get_align(t_wrap_n), pos, level);

    pos.x = -634;
    pos.y = 354;
    text_draw(t_info, s_text, mat4_mul(camera_get_vp(c_main), mat4_translate(mat4_ident, pos)));

    window_redraw(win);
    return !window_get_should_close(win);
}

int main(int argc, char** argv) {
    win = window_new(1280, 720, false, "Heightmap Demo");
    c_main = window_create_2d_camera(win);

    init_base_resource_path(NULL);
    shaders_init();
    s_text = shader_basic_tex_get();

    char* path = assets_path("OpenSans-Regular.ttf", NULL);
    f_text = load_font(path, 16);
    t_wrap_c = text_new(f_text, demo_str);
    text_set_wrap(t_wrap_c, TEXT_WRAP_CHARACTER);
    text_set_align(t_wrap_c, TEXT_ALIGN_TOP);

    t_wrap_w = text_new(f_text, demo_str);
    text_set_align(t_wrap_w, TEXT_ALIGN_TOP);
    text_set_wrap(t_wrap_w, TEXT_WRAP_WORD);

    t_wrap_n = text_new(f_text, demo_str);
    text_set_align(t_wrap_n, TEXT_ALIGN_TOP);

    t_info = text_new(f_text, info_str);
    text_set_align(t_info, TEXT_ALIGN_BOTTOM_LEFT);
    sfree(path);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(int i = 0; i < ALIGN_KEYS_COUNT; ++i) {
        input_add_key_action(keys[i].key, as_event(action_event, wrap, (void*)keys[i].alignment));
    }

    input_add_key_action(K_W, as_event(action_event, wrap, (void*)TEXT_ALIGN_TOP));
    input_add_key_action(K_E, as_event(action_event, wrap, (void*)TEXT_ALIGN_TOP_RIGHT));
    input_add_key_action(K_A, as_event(action_event, wrap, (void*)TEXT_ALIGN_LEFT));
    input_add_key_action(K_S, as_event(action_event, wrap, (void*)TEXT_ALIGN_CENTER));
    input_add_key_action(K_D, as_event(action_event, wrap, (void*)TEXT_ALIGN_RIGHT));
    input_add_key_action(K_Z, as_event(action_event, wrap, (void*)TEXT_ALIGN_BOTTOM_LEFT));
    input_add_key_action(K_X, as_event(action_event, wrap, (void*)TEXT_ALIGN_BOTTOM));
    input_add_key_action(K_C, as_event(action_event, wrap, (void*)TEXT_ALIGN_BOTTOM_RIGHT));

    mainloop_create_run(loop);

    text_free(t_wrap_c, false);
    text_free(t_wrap_w, false);
    text_free(t_wrap_n, false);
    font_free(f_text);
    resource_path_free();

    camera_free(c_main);
    window_free_final(win);
    shaders_cleanup();

    return 0;
}
