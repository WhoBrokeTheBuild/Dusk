#version 450 core

#include "Dusk/TransformData.inc.glsl"
#include "Dusk/VertexAttributes.inc.glsl"

layout(location = 0) out vec4 v_Position;
layout(location = 1) out vec4 v_Normal;
layout(location = 2) out vec4 v_Color;
layout(location = 3) out vec2 v_UV;

void main()
{
    gl_Position = u_TransformData.MVP * a_Position;
    v_Position = a_Position;
    v_Normal = a_Normal;
    v_Color = a_Color1;
    v_UV = a_UV1;
}