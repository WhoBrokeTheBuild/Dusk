#version 450 core

#include <Dusk/Transform.inc.glsl>
#include <Dusk/VertexAttributes.inc.glsl>

layout(location = 0) out vec4 v_Color;

void main()
{
    v_Color = a_Color;

    gl_Position = u_MVP * vec4(a_Position.xyz, 1.0);
}