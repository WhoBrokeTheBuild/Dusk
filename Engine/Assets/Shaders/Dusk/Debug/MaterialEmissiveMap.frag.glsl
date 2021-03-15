
#version 450 core

#include <Dusk/Material.inc.glsl>
#include <Dusk/ColorSpace.inc.glsl>

layout(location = 0) in vec2 v_TexCoord;

layout(location = 0) out vec4 o_Color;

void main()
{
    o_Color = vec4(texture(u_EmissiveMap, v_TexCoord).rgb, 1);
}
