#version 450 core

#include <Dusk/Globals.inc.glsl>
#include <Dusk/Material.inc.glsl>
#include <Dusk/Math.inc.glsl>
#include <Dusk/ColorSpace.inc.glsl>

// Index Of Refraction
const float c_IOR = 1.5;

// Fresnel reflactance at 0 degrees (looking straight on)
const vec3 c_F0 = vec3(((c_IOR - 1.0) * (c_IOR - 1.0)) / ((c_IOR + 1.0) * (c_IOR + 1)));

// 
const float c_ReflectStrength = 64.0;

// Gamma correction
float c_Gamma = 2.2;

// TODO: Move
const vec3 c_LightColor = vec3(1.0);
const vec3 c_LightPosition = vec3(2.0, 2.0, 2.0);
const vec3 c_EyePosition = vec3(2.0, 3.0, 2.0);

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in mat3 v_TBN;

layout(location = 0) out vec4 o_Color;

void main() 
{
    vec4 BaseColor = SRGBToLinear(texture(u_BaseColorMap, v_TexCoord)) * u_BaseColorFactor;

    // Separate roughness/metallic out of green/blue respectively
    vec4 metallicRoughness = texture(u_MetallicRoughnessMap, v_TexCoord);
    float Roughness = clamp(metallicRoughness.g * u_RoughnessFactor, 0.04, 1.0);
    float Metallic = clamp(metallicRoughness.b * u_MetallicFactor, 0.0, 1.0);

    vec3 Reflectance0 = mix(c_F0, BaseColor.rgb, Metallic);
    float ref = max(max(Reflectance0.r, Reflectance0.g), Reflectance0.b);
    vec3 Reflectance90 = vec3(clamp(ref * c_ReflectStrength, 0.0, 1.0));

    // Multiply the normal map by the normal from TBN
    vec3 N = (texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0) * v_TBN[2];
    vec3 V = normalize(c_EyePosition - v_Position);
    vec3 L = normalize(c_LightPosition - v_Position);
    vec3 H = normalize(L + V);

    vec3 reflection = -normalize(reflect(V, N));

    float R = Roughness * Roughness;
    float R2 = R * R;

    // Precompiled dots
    float NdotL = clamp(    dot(N, L),  0.001, 1.0);
    float NdotV = clamp(abs(dot(N, V)), 0.001, 1.0);
    float NdotH = clamp(    dot(N, H),  0.0,   1.0);
    float LdotH = clamp(    dot(L, H),  0.0,   1.0);
    float VdotH = clamp(    dot(V, H),  0.0,   1.0);

    // Specular Reflection
    vec3 F = Reflectance0 + (Reflectance90 - Reflectance0) * pow(clamp(1.0 - VdotH, 0.0, 1.0), 5.0);

    // Geometric Occlusion
    float attenuationL = 2.0 * NdotL / (NdotL + sqrt(R2 + (1.0 - R2) * (NdotL * NdotL)));
    float attenuationV = 2.0 * NdotV / (NdotV + sqrt(R2 + (1.0 - R2) * (NdotV * NdotV)));
    float G = attenuationL * attenuationV;

    // Microfacet Distribution
    float facet = (NdotH * R2 - NdotH) * NdotH + 1.0;
    float D = R2 / (M_PI * facet * facet);

    // Compute color from the light color, diffuse, and specular
    vec3 diffuseColor = (BaseColor.rgb * (1.0 - c_F0)) * (1.0 - Metallic);
    vec3 diffuseContrib = (1.0 - F) * (diffuseColor / M_PI);
    vec3 specularContrib = F * G * D / (4.0 * NdotL * NdotV);
    vec3 Color = NdotL * c_LightColor * (diffuseContrib + specularContrib);

    // Ambient Occlusion
    float AO = texture(u_OcclusionMap, v_TexCoord).r;
    Color = mix(Color, Color * AO, u_OcclusionStrength);

    // Emissive
    vec3 Emissive = SRGBToLinear(texture(u_EmissiveMap, v_TexCoord)).rgb * u_EmissiveFactor;
    Color += Emissive;

    o_Color = vec4(pow(Color, vec3(1.0 / c_Gamma)), BaseColor.a);
}