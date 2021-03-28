
#version 450 core

#include <Dusk/Transform.inc.glsl>
#include <Dusk/VertexAttributes.inc.glsl>

layout(location = 0) out vec4 v_Color;

void main() {
    gl_Position = u_MVP * a_Position;
    v_Color = vec4(a_TexCoord1, 0, 1);
}
