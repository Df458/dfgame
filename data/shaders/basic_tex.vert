#version 330
uniform mat4 transform;
uniform vec2 uv_offset = vec2(0, 0);
uniform vec2 uv_scale = vec2(1, 1);

in vec3 i_pos;
in vec2 i_uv;

out vec2 v_uv;

void main() {
    gl_Position = transform * vec4(i_pos, 1.0);
    v_uv = (i_uv * uv_scale) + uv_offset;
}
