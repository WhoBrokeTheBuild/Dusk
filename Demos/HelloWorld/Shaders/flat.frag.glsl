#version 450 core

layout(location = 0) in vec4 v_Position;
layout(location = 1) in vec4 v_Normal;
layout(location = 2) in vec4 v_Color;
layout(location = 3) in vec2 v_UV;

layout(location = 0) out vec4 o_Color;

void main() 
{
    o_Color = vec4(v_Normal.xyz, 1.0);
}