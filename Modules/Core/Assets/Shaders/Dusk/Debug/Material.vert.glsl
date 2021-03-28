
#version 450 core

#include <Dusk/Transform.inc.glsl>
#include <Dusk/VertexAttributes.inc.glsl>

layout(location = 0) out vec2 v_TexCoord;

void main() {
    gl_Position = u_MVP * a_Position;
    v_TexCoord = a_TexCoord1;
}
