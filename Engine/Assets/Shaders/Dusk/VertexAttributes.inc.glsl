#ifndef VERTEX_ATTRIBUTES_INC_GLSL
#define VERTEX_ATTRIBUTES_INC_GLSL

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec4 a_Tangent;
layout(location = 3) in vec4 a_Bitangent;
layout(location = 4) in vec4 a_Color1;
layout(location = 5) in vec4 a_Color2;
layout(location = 6, component = 0) in vec2 a_UV1;
layout(location = 6, component = 2) in vec2 a_UV2;
layout(location = 7, component = 0) in vec2 a_UV3;
layout(location = 7, component = 2) in vec2 a_UV4;
layout(location = 8, component = 0) in vec2 a_UV5;
layout(location = 8, component = 2) in vec2 a_UV6;

#endif // VERTEX_ATTRIBUTES_INC_GLSL
