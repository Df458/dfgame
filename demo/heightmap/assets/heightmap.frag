#version 330
uniform vec4 u_color = vec4(1, 1, 1, 1);
uniform sampler2D u_texture;
in float v_offset;
in vec3 v_normal;
in vec2 v_uv;

layout(location = 0) out vec4 f_color;

void main() {
    vec3 v_color = vec3(0, 1, 0);
    if(v_offset < 1.1)
        v_color = vec3(1, 1, 0) * (v_offset - 1) * 10;
    else if(v_offset < 1.15)
        v_color = vec3(1, 1, 0);
    else if(v_offset > 1.4 && v_offset < 1.7)
        v_color = vec3(0.5);
    else if(v_offset > 1.4)
        v_color = vec3(1);

    vec3 u = vec3(v_uv.x, length(texture2D(u_texture, (v_uv + vec2( 0, -1)) * 0.01)) * 30, v_uv.y);
    vec3 d = vec3(v_uv.x, length(texture2D(u_texture, (v_uv + vec2( 0,  1)) * 0.01)) * 30, v_uv.y);
    vec3 l = vec3(v_uv.x, length(texture2D(u_texture, (v_uv + vec2(-1,  0)) * 0.01)) * 30, v_uv.y);
    vec3 r = vec3(v_uv.x, length(texture2D(u_texture, (v_uv + vec2( 1,  0)) * 0.01)) * 30, v_uv.y);
    vec3 c = vec3(v_uv.x, length(texture2D(u_texture, (v_uv) * 0.01)) * 30, v_uv.y);
    vec3 v_normal = normalize(vec3((l.y - c.y) + (c.y - r.y), 4, (u.y - c.y) + (c.y - d.y)));

    f_color = u_color * vec4(v_color * dot(v_normal, normalize(vec3(1, 1, 0))), 1);
}
