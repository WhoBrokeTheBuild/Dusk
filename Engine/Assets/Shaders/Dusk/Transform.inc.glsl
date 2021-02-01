#ifndef DUSK_TRANSFORM_INC_GLSL
#define DUSK_TRANSFORM_INC_GLSL

layout(binding = 1, std140) uniform DuskTransform
{
    mat4 Model;
    mat4 View;
    mat4 Proj;
    mat4 MVP;

};

#endif // DUSK_TRANSFORM_INC_GLSL