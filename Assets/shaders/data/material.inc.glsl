layout(std140) uniform DuskMaterialData
{
    vec3 Ambient;
    float Roughness;
    vec3 Diffuse;
    float Metallic;
    vec3 Specular;
    float SpecularExponent;
    vec3 Emission;
    float Dissolve;
    float Sheen;
    float ClearcoatThickness;
    float ClearcoatRoughness;
    float Anisotropy;
    float AnisotropyRotation;

    uint MapFlags;

} _MaterialData;

const uint AmbientMapFlag  = 1u; // 00001
const uint DiffuseMapFlag  = 2u; // 00010
const uint SpecularMapFlag = 4u; // 00100
const uint NormalMapFlag   = 8u; // 01000

uniform sampler2D _AmbientMap;
uniform sampler2D _DiffuseMap;
uniform sampler2D _SpecularMap;
uniform sampler2D _NormalMap;

bool HasAmbientMap() {
    return ((_MaterialData.MapFlags & AmbientMapFlag) > 0u);
}
bool HasDiffuseMap() {
    return ((_MaterialData.MapFlags & DiffuseMapFlag) > 0u);
}
bool HasSpecularMap() {
    return ((_MaterialData.MapFlags & SpecularMapFlag) > 0u);
}
bool HasNormalMap() {
    return ((_MaterialData.MapFlags & NormalMapFlag) > 0u);
}
