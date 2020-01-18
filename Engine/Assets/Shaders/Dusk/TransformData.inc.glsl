#ifndef DUSK_TRANSFORM_DATA_INC_GLSL
#define DUSK_TRANSFORM_DATA_INC_GLSL

layout(binding = 0, std140) uniform DuskTransformData
{
    mat4 Model;
    mat4 View;
    mat4 Proj;
    mat4 MVP;

} u_TransformData;

#endif // DUSK_TRANSFORM_DATA_INC_GLSL