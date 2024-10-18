#ifndef DUSK_TRANSFORM_INC_GLSL
#define DUSK_TRANSFORM_INC_GLSL

layout(binding = 2, std140) uniform DuskMaterial
{
    // Base Color Factor
    vec4 u_BaseColorFactor;

    // Emissive Factor
    vec3 u_EmissiveFactor;

    // Metallic Factor
    float u_MetallicFactor;

    // Roughness Factor
    float u_RoughnessFactor;

    // Ambient Occlusion Strength
    float u_OcclusionStrength;

    // Normal Scale
    float u_NormalScale;
};

// Base Color Texture Map
layout(set=1, binding=0) uniform sampler2D u_BaseColorMap;

// Normal Texture Map
layout(set=1, binding=1) uniform sampler2D u_NormalMap;

// Metallic/Roughness Texture Map
//   metallic = u_MetallicRoughnessMap.g;
//   roughness = u_MetallicRoughnessMap.b;
layout(set=1, binding=2) uniform sampler2D u_MetallicRoughnessMap;

// Emissive Texture Map
layout(set=1, binding=3) uniform sampler2D u_EmissiveMap;

// Ambient Occlusion Texture Map
layout(set=1, binding=4) uniform sampler2D u_OcclusionMap;

#endif // DUSK_TRANSFORM_INC_GLSL