#version 450 core

#include "Dusk/TransformData.inc.glsl"
#include "Dusk/VertexAttributes.inc.glsl"

layout(location = 0) out vec4 v_Color;

void main() 
{
    v_Color = a_Color1;
    gl_Position = u_TransformData.MVP * a_Position;
}