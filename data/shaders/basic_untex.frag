#version 330
uniform vec4 u_color = vec4(1, 1, 1, 1);
in vec3 v_color;

layout(location = 0) out vec4 f_color;

void main() {
    f_color = u_color * vec4(v_color, 1);
}
