#version 450 core

#include <Dusk/Globals.inc.glsl>
#include <Dusk/Transform.inc.glsl>
#include <Dusk/VertexAttributes.inc.glsl>

layout(location = 0) out vec4 v_Normal;

void main()
{
    gl_Position = u_MVP * a_Position;
    v_Normal = a_Normal;
}