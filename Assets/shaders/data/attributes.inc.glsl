#ifndef ATTRIBUTES_INC
#define ATTRIBUTES_INC

layout(location = 0) in vec4 a_Position;

#ifdef HAS_NORMAL
layout(location = 1) in vec3 a_Normal;
#endif // HAS_NORMAL

#ifdef HAS_UV
layout(location = 2) in vec2 a_UV;
#endif // HAS_UV

#ifdef HAS_COLOR
layout(location = 3) in vec4 a_Color;
#endif // HAS_COLOR

#ifdef HAS_TANGENT
layout(location = 4) in vec4 a_Tangent;
#endif // HAS_TANGENT

#endif // ATTRIBUTES_INC
