#version 330
uniform mat4 transform;
uniform sampler2D u_texture;

in  vec3 i_pos;
out float v_offset;
out vec3 v_normal;
out vec2 v_uv;

void main() {
    v_uv = i_pos.xz;
    v_offset = length(texture2D(u_texture, v_uv * 0.01));

    /* vec3 up     = normalize(vec3(i_pos.x    , length(texture2D(u_texture, (i_pos.xz + vec2( 0,  1)) * 0.01).xyz) * 30, i_pos.z + 1)); */
    /* vec3 down   = normalize(vec3(i_pos.x    , length(texture2D(u_texture, (i_pos.xz + vec2( 0, -1)) * 0.01).xyz) * 30, i_pos.z - 1)); */
    /* vec3 left   = normalize(vec3(i_pos.x - 1, length(texture2D(u_texture, (i_pos.xz + vec2(-1,  0)) * 0.01).xyz) * 30, i_pos.z)); */
    /* vec3 right  = normalize(vec3(i_pos.x + 1, length(texture2D(u_texture, (i_pos.xz + vec2( 1,  0)) * 0.01).xyz) * 30, i_pos.z)); */
    /* v_normal = normalize(cross(up - down, vec3(1, 0, 0)) + cross(left - right, vec3(1, 0, 0))); */

    gl_Position = transform * (vec4(i_pos, 1.0) + vec4(0, v_offset * 30, 0, 1));
}
