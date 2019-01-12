#version 330
uniform mat4 u_transform;

in  vec3 i_pos;

void main() {
    gl_Position = u_transform * vec4(i_pos, 1.0);
}
