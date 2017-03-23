#version 330
uniform sampler2D u_texture;
uniform vec4 u_color = vec4(1, 1, 1, 1);

in vec2 v_uv;

layout(location = 0) out vec4 f_color;

void main() {
    f_color = texture2D(u_texture, v_uv) * u_color;
}
