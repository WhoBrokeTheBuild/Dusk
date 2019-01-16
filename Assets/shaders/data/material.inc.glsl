#ifndef MATERIAL_INC
#define MATERIAL_INC

uniform vec4 u_BaseColorFactor;

#ifdef HAS_BASE_COLOR_MAP
uniform sampler2D u_BaseColorMap;
#endif // HAS_BASE_COLOR_MAP

#ifdef HAS_NORMAL_MAP
uniform sampler2D u_NormalMap;
uniform float u_NormalScale;
#endif // HAS_NORMAL_MAP

uniform float u_MetallicFactor;
uniform float u_RoughnessFactor;

#ifdef HAS_METALLIC_ROUGHNESS_MAP
uniform sampler2D u_MetallicRoughnessMap;
#endif // HAS_METALLIC_ROUGHNESS_MAP

uniform vec3 u_EmissiveFactor;

#ifdef HAS_EMISSIVE_MAP
uniform sampler2D u_EmissiveMap;
#endif // HAS_EMISSIVE_MAP

#ifdef HAS_OCCLUSION_MAP
uniform sampler2D u_OcclusionMap;
uniform float u_OcclusionStrength;
#endif // HAS_OCCLUSION_MAP

#endif // MATERIAL_INC
