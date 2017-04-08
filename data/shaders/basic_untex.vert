#version 330
uniform mat4 transform;

in  vec3 i_pos;
in  vec3 i_color;
out vec3 v_color;

void main() {
    gl_Position = transform * vec4(i_pos, 1.0);
    v_color = i_color;
}
