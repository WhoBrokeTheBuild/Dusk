#version 450 core

layout(location = 0) in vec4 v_Color;

layout(location = 0) out vec4 _Color;

void main() 
{
    _Color = v_Color;
}