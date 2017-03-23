#version 330
uniform mat4 transform;

in  vec3 i_pos;

void main() {
    gl_Position = transform * vec4(i_pos, 1.0);
}
