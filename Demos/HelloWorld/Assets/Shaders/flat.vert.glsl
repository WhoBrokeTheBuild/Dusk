#version 450 core

#include "Dusk/TransformData.inc.glsl"
#include "Dusk/VertexAttributes.inc.glsl"

layout(location = 1) out vec4 v_Normal;

void main()
{
    gl_Position = MVP * a_Position;
    v_Normal = a_Normal;
}