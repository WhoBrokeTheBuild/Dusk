#version 450 core

#include <Dusk/ColorSpace.inc.glsl>

layout(location = 0) in vec4 v_Color;

layout(location = 0) out vec4 o_Color;

void main()
{
    o_Color = SRGBToLinear(v_Color);
}
