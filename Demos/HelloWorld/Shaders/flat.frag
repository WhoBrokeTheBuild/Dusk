
layout(location = 1) in vec4 v_Position;

layout(location = 2) out vec4 o_Color;

void main() 
{
    o_Color = vec4(v_Position.xyz, 1.0);
}