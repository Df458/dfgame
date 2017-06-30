#version 330
uniform sampler2D u_texture;
in vec2 v_uv;

layout(location = 0) out vec4 f_color;

void main() {
    f_color = vec4(0);
    for(int i = -5; i < 5; ++i) {
        for(int j = -5; j < 5; ++j) {
            f_color += texture2D(u_texture, vec2(v_uv.x + (i * 0.001), v_uv.y + (j * 0.001)));
        }
    }
    f_color = vec4(f_color.xyz * 0.01, 1);
}
