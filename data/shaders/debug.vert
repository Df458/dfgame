#version 330
uniform mat4 u_transform;

in vec3 i_pos;
in vec4 i_color;

out vec4 v_color;

void main() {
    gl_Position = u_transform * vec4(i_pos, 1.0);
    v_color = i_color;
}
