
#version 450 core

#include <Dusk/Transform.inc.glsl>
#include <Dusk/VertexAttributes.inc.glsl>
#include <Dusk/TBN.inc.glsl>

layout(location = 0) out vec2 v_TexCoord;
layout(location = 1) out mat3 v_TBN;

void main() {
    gl_Position = u_MVP * a_Position;
    v_TexCoord = a_TexCoord1;
    v_TBN = CalculateTBN(u_Model, a_Normal, a_Tangent);
}
