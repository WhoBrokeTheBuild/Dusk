#ifndef DUSK_TRANSFORM_INC_GLSL
#define DUSK_TRANSFORM_INC_GLSL

layout(binding = 2, std140) uniform DuskMaterial
{
    vec4 BaseColorFactor;
    vec3 EmissiveFactor;
    float MetallicFactor;
    float RoughnessFactor;
    float OcclusionStrength;
    float NormalScale;
};

layout(binding = 0) sampler2D BaseColorMap;
layout(binding = 1) sampler2D NormalMap;
layout(binding = 2) sampler2D MetallicRoughnessMap;
layout(binding = 3) sampler2D EmissiveMap;
layout(binding = 4) sampler2D OcclusionMap;

#endif // DUSK_TRANSFORM_INC_GLSL