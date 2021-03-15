
#version 450 core

#include <Dusk/Material.inc.glsl>

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in mat3 v_TBN;

layout (location = 0) out vec4 o_Color;

void main() {
    vec3 N = (2.0 * texture(u_NormalMap, v_TexCoord).rgb - 1.0) * u_NormalScale;
    o_Color = vec4(normalize(v_TBN * N), 1);
}
