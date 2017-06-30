#version 330
uniform sampler2D u_texture;
in vec2 v_uv;

layout(location = 0) out vec4 f_color;

void main() {
    f_color = vec4(vec3(1, 1, 1) - texture2D(u_texture, v_uv).xyz, 1);
}
