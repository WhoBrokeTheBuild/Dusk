
layout(location = 0) in vec4 v_Position;

layout(location = 0) out vec4 o_Color;

void main() 
{
    o_Color = vec4(v_Position.xyz, 1.0);
}