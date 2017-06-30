#version 330
uniform sampler2D u_texture;
uniform float u_time;
in vec2 v_uv;

layout(location = 0) out vec4 f_color;

void main() {
    f_color = texture2D(u_texture, vec2(v_uv.x, v_uv.y + (sin(u_time + v_uv.x * 25) * 0.01)));
}
