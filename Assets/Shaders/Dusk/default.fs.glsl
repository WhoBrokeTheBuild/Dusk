#include shaders/data/transform.inc.glsl
#include shaders/data/material.inc.glsl

const float c_IOR = 1.0;
const float c_MinRoughness = 0.04;
const vec3 c_LightColor = vec3(1.0);
uniform vec3 c_LightDirection = vec3(0.0, 0.5, 0.5);

uniform vec3 u_Camera;

in vec3 v_Position;
in vec2 v_UV;

#ifdef HAS_NORMAL
#ifdef HAS_TANGENT
in mat3 v_TBN;
#else
in vec3 v_Normal;
#endif // HAS_TANGENT
#endif // HAS_NORMAL

out vec4 _Color;

struct PBRInfo
{
    float NdotL;                  // cos angle between normal and light direction
    float NdotV;                  // cos angle between normal and view direction
    float NdotH;                  // cos angle between normal and half vector
    float LdotH;                  // cos angle between light direction and half vector
    float VdotH;                  // cos angle between view direction and half vector
    float perceptualRoughness;    // roughness value, as authored by the model creator (input to shader)
    float metallic;               // metallic value at the surface
    vec3 reflectance0;            // full reflectance color (normal incidence angle)
    vec3 reflectance90;           // reflectance color at grazing angle
    float alphaRoughness;         // roughness mapped to a more linear change in the roughness (proposed by [2])
    vec3 diffuseColor;            // color contribution from diffuse lighting
    vec3 specularColor;           // color contribution from specular lighting
};

vec4 SRGBtoLinear(vec4 srgb) {
    vec3 linear = pow(srgb.xyz, vec3(2.2));
    return vec4(linear, srgb.w);
}

vec3 GetNormal() {
#ifdef HAS_TANGENT
    mat3 tbn = v_TBN;
#else
    vec3 pos_dx = dFdx(v_Position);
    vec3 pos_dy = dFdy(v_Position);
    vec3 uv_dx = dFdx(vec3(v_UV, 0.0));
    vec3 uv_dy = dFdy(vec3(v_UV, 0.0));
    vec3 t = (uv_dy.t * pos_dx - uv_dx.t * pos_dy) / (uv_dx.s * uv_dy.t - uv_dy.s * uv_dx.t);

#ifdef HAS_NORMAL
    vec3 ng = normalize(v_Normal);
#else
    vec3 ng = cross(pos_dx, pos_dy);
#endif // HAS_NORMALS

    t = normalize(t - ng * dot(ng, t));
    vec3 b = normalize(cross(ng, t));
    mat3 tbn = mat3(t, b, ng);
#endif // HAS_TANGENT
    
#ifdef HAS_NORMAL_MAP
    vec3 n = texture2D(u_NormalMap, v_UV).rgb;
    n = normalize(tbn * ((2.0 * n - 1.0) * vec3(0, 0, 1.0)));
#else
    vec3 n = normalize(tbn[2].xyz);
#endif // HAS_NORMAL_MAP

    return n;
}

vec3 GetDiffuse(PBRInfo pbr) {
    return pbr.diffuseColor / M_PI;
}

vec3 GetSpecularReflection(PBRInfo pbr) {
    return pbr.reflectance0 + (pbr.reflectance90 - pbr.reflectance0) * pow(clamp(1.0 - pbr.VdotH, 0.0, 1.0), 5.0);
}

float GetGeometricOcclusion(PBRInfo pbr) {
    float r = pbr.alphaRoughness;
    float attenuationL = 2.0 * pbr.NdotL / (pbr.NdotL + sqrt(r * r + (1.0 - r * r) * (pbr.NdotL * pbr.NdotL)));
    float attenuationV = 2.0 * pbr.NdotV / (pbr.NdotV + sqrt(r * r + (1.0 - r * r) * (pbr.NdotV * pbr.NdotV)));
    return attenuationL * attenuationV;
}

float GetMicrofacetDistribution(PBRInfo pbr) {
    float roughnessSq = pbr.alphaRoughness * pbr.alphaRoughness;
    float f = (pbr.NdotH * roughnessSq - pbr.NdotH) * pbr.NdotH + 1.0;
    return roughnessSq / (M_PI * f * f);
}

void main() {
    PBRInfo pbr;

    pbr.perceptualRoughness = u_RoughnessFactor;
    pbr.metallic = u_MetallicFactor;

#ifdef HAS_METALLIC_ROUGHNESS_MAP
    // Unpack Metallic and Roughness values from G and B channels
    vec4 tmp = texture2D(u_MetallicRoughnessMap, v_UV);
    pbr.perceptualRoughness = tmp.g * pbr.perceptualRoughness;
    pbr.metallic = tmp.b * pbr.metallic;
#endif // HAS_METALLIC_ROUGHNESS_MAP

    pbr.perceptualRoughness = clamp(pbr.perceptualRoughness, c_MinRoughness, 1.0);
    pbr.metallic = clamp(pbr.metallic, 0.0, 1.0);

    // Convert to material roughness by squaring the perceptual roughness
    pbr.alphaRoughness = pbr.perceptualRoughness * pbr.perceptualRoughness;

#ifdef HAS_BASE_COLOR_MAP
    vec4 baseColor = SRGBtoLinear(texture2D(u_BaseColorMap, v_UV)) * u_BaseColorFactor;
#else
    vec4 baseColor = u_BaseColorFactor;
#endif // HAS_BASE_COLOR_MAP

    //vec3 f0 = vec3(((c_IOR - 1.0) * (c_IOR - 1.0)) / ((c_IOR + 1) * (c_IOR + 1)));
    vec3 f0 = vec3(0.04);
    pbr.diffuseColor = baseColor.rgb * (vec3(1.0) - f0);
    pbr.diffuseColor *= 1.0 - pbr.metallic;
    pbr.specularColor = mix(f0, baseColor.rgb, pbr.metallic);

    float reflectance = max(max(pbr.specularColor.r, pbr.specularColor.g), pbr.specularColor.b);
    float reflectance90 = clamp(reflectance * 25.0, 0.0, 1.0);
    pbr.reflectance0 = pbr.specularColor.rgb;
    pbr.reflectance90 = vec3(1.0) * reflectance90;

    vec3 n = GetNormal();
    vec3 v = normalize(u_Camera - v_Position);
    vec3 l = normalize(c_LightDirection);
    vec3 h = normalize(l + v);
    vec3 reflection = -normalize(reflect(v, n));

    pbr.NdotL = clamp(dot(n, l), 0.001, 1.0);
    pbr.NdotV = clamp(abs(dot(n, v)), 0.001, 1.0);
    pbr.NdotH = clamp(dot(n, h), 0.0, 1.0);
    pbr.LdotH = clamp(dot(l, h), 0.0, 1.0);
    pbr.VdotH = clamp(dot(v, h), 0.0, 1.0);

    vec3 F = GetSpecularReflection(pbr);
    float G = GetGeometricOcclusion(pbr);
    float D = GetMicrofacetDistribution(pbr);

    vec3 diffuseContrib = (1.0 - F) * GetDiffuse(pbr);
    vec3 specularContrib = F * G * D / (4.0 * pbr.NdotL * pbr.NdotV);
    vec3 color = pbr.NdotL * c_LightColor * (diffuseContrib + specularContrib);

#ifdef HAS_OCCLUSION_MAP
    float ao = texture2D(u_OcclusionMap, v_UV).r;
    color = mix(color, color * ao, u_OcclusionStrength);
#endif // HAS_OCCLUSION_MAP

#ifdef HAS_EMISSIVE_MAP
    vec3 emissive = SRGBtoLinear(texture2D(u_EmissiveMap, v_UV)).rgb * u_EmissiveFactor;
    color += emissive;
#endif // HAS_EMISSIVE_MAP

    _Color = vec4(pow(color, vec3(1.0 / 2.2)), baseColor.a);
}
