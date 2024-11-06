#version 450 core

#include <Dusk/Globals.inc.glsl>
#include <Dusk/Transform.inc.glsl>
#include <Dusk/VertexAttributes.inc.glsl>
#include <Dusk/TBN.inc.glsl>

layout(location = 0) out vec3 v_Position;
layout(location = 1) out vec4 v_Color;
layout(location = 2) out vec2 v_TexCoord;
layout(location = 3) out mat3 v_TBN;

void main()
{
    vec4 position = u_Model * a_Position;
    v_Position = position.xyz / position.w;

    v_Color = a_Color;
    v_TexCoord = a_TexCoord;

    v_TBN = CalculateTBN(u_Model, a_Normal, a_Tangent);

    gl_Position = u_MVP * vec4(a_Position.xyz, 1.0);
}