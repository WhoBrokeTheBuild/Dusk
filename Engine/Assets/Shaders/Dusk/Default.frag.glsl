#version 450 core

#include <Dusk/Globals.inc.glsl>
#include <Dusk/Material.inc.glsl>
#include <Dusk/Math.inc.glsl>
#include <Dusk/ColorSpace.inc.glsl>

// 
const float c_Epsilon = 0.00001;

// Constant normal incidence Fresnel factor for all dielectrics
const vec3 c_FresnelDielectric = vec3(0.04);

// Gamma correction
float c_Gamma = 2.2;

// TODO: Move
const vec3 c_LightRadiance = vec3(0.2);
const vec3 c_EyePosition = vec3(2.0, 3.0, 2.0);
const vec3 c_LightDirection = -c_EyePosition;

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in mat3 v_TBN;

layout(location = 0) out vec4 o_Color;

// Shlick's approximation of the Fresnel factor
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// GGX/Towbridge-Reitz normal distribution functions
// Use Disney's reparamaterization of (alpha = roughness^2)
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (M_PI * denom * denom);
}

// Single term for separable Schlick-GXX (see gaSchlickGGX)
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

void main() 
{
    vec3 albedo = SRGBToLinear(texture(u_BaseColorMap, v_TexCoord)).rgb;

    vec3 metallicRoughness = texture(u_MetallicRoughnessMap, v_TexCoord).rgb;
    float roughness = metallicRoughness.g;
    float metallic = metallicRoughness.b;

    // Outgoing light direction (vector from world space fragment position to the eye)
    vec3 Lo = normalize(c_EyePosition - v_Position);

    // Get current normal and transform to world space
    vec3 N = normalize(2.0 * texture(u_NormalMap, v_TexCoord).rgb - 1.0);
    N = normalize(v_TBN[2] * N);

    // Angle between surface normal and outgoing light direction
    float cosLo = max(0.0, dot(N, Lo));

    // Specular reflection vector
    vec3 Lr = 2.0 * cosLo * N - Lo;

    // Fresnel reflectance at normal incidence (for metals use albedo color)
    vec3 F0 = mix(c_FresnelDielectric, albedo, metallic);

    vec3 directLighting = vec3(0.0);
    // foreach Light
    {
        vec3 Li = -c_LightDirection;
        vec3 Lradiance = c_LightRadiance;

        // Half-vector between Li and Lo
        vec3 Lh = normalize(Li + Lo);

        // Calculate angles between surface normal and light vectors
        float cosLi = max(0.0, dot(N, Li));
        float cosLh = max(0.0, dot(N, Lh));

        // Calculate Fresnel term for direct lighting
        vec3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));

        // Calculate normal distribution for specular BRDF
        float D = ndfGGX(cosLh, roughness);

        // Calculate geometric attenuation for specular BRDF
        float G = gaSchlickGGX(cosLi, cosLo, roughness);

        // Diffuse scattering happens due to light being refracted multiple times by a dielectric medium
        // Metals on the other hand, either reflect or absorb energy, so diffuse contribution is always zero
        // To be energy conserving, we must scale diffuse BRDF contribution based on Fresnel factor and metalness
        vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metallic);

        // Lambert diffuse BRDF
        // We don't scale by 1/PI for lighting and material units to be more convenient
        // See https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
        vec3 diffuseBRDF = kd * albedo;

        // Cook-Torrance specular microfacet BRDF
        vec3 specularBRDF = (F * D * G) / max(c_Epsilon, 4.0 * cosLi * cosLo);

        // Total contribution for this light
        directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
    }

    vec3 ambientLighting = vec3(0.0);
    {
        // Sample diffuse irradiance at normal direction
        // TODO:

        vec3 Li = -c_LightDirection;

        // Half-vector between Li and Lo
        vec3 Lh = normalize(Li + Lo);

        // Calculate angles between surface normal and light vectors
        float cosLi = max(0.0, dot(N, Li));
        float cosLh = max(0.0, dot(N, Lh));



        // Calculate Fresnel term for ambient lighting
        vec3 F = fresnelSchlick(F0, cosLh);

        // Get diffuse contribution factor (as with direct lighting)
        vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metallic);

        // Irradiance?
        vec3 diffuseIBL = kd * albedo; // * irradiance

        // TODO:

        ambientLighting = diffuseIBL;
    }

    vec3 color = directLighting + ambientLighting;

    // Ambient Occlusion
    float AO = texture(u_OcclusionMap, v_TexCoord).r;
    color = mix(color, color * AO, u_OcclusionStrength);

    // Emissive
    vec3 emissiveLighting = SRGBToLinear(texture(u_EmissiveMap, v_TexCoord)).rgb * u_EmissiveFactor;
    color += emissiveLighting;

    o_Color = vec4(color, 1.0);
}