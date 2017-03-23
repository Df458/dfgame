#version 330
uniform sampler2D texture;
uniform vec4 color;
in  vec2 v_uv;
layout(location = 0) out vec4 f_color;

void main() {
    f_color = color;
    f_color.a = texture2D(texture, v_uv).a;
}
