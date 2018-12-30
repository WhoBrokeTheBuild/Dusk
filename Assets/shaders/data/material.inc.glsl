vec3 Ambient;
vec3 Diffuse;
vec3 Specular;
vec3 Emission;

float Roughness;
float Metallic;
float Shininess;
float Dissolve;
float Sheen;
float ClearcoatThickness;
float ClearcoatRoughness;
float Anisotropy;
float AnisotropyRotation;

uint TextureFlags;

const uint HAS_AMBIENT_MAP      = 1 << 0;
const uint HAS_DIFFUSE_MAP      = 1 << 1;
const uint HAS_SPECULAR_MAP     = 1 << 2;
const uint HAS_NORMAL_MAP       = 1 << 3;
const uint HAS_ALPHA_MAP        = 1 << 4;
const uint HAS_DISPLACEMENT_MAP = 1 << 5;
const uint HAS_ROUGHNESS_MAP    = 1 << 6;
const uint HAS_METALLIC_MAP     = 1 << 7;
const uint HAS_SHEEN_MAP        = 1 << 8;
const uint HAS_EMISSIVE_MAP     = 1 << 9;

uniform sampler AmbientMap;
uniform sampler DiffuseMap;
uniform sampler SpecularMap;
uniform sampler NormalMap;
uniform sampler AlphaMap;
uniform sampler DisplacementMap;
uniform sampler RoughnessMap;
uniform sampler MetallicMap;
uniform sampler SheenMap;
uniform sampler EmissiveMap;

bool HasAmbientMap() {
    return ((TextureFlags & HAS_AMBIENT_MAP) > 0u);
}

bool HasDiffuseMap() {
    return ((TextureFlags & HAS_DIFFUSE_MAP) > 0u);
}

bool HasSpecularMap() {
    return ((TextureFlags & HAS_SPECULAR_MAP) > 0u);
}

bool HasNormalMap() {
    return ((TextureFlags & HAS_NORMAL_MAP) > 0u);
}

bool HasAlphaMap() {
    return ((TextureFlags & HAS_ALPHA_MAP) > 0u);
}

bool HasDisplacementMap() {
    return ((TextureFlags & HAS_DISPLACEMENT_MAP) > 0u);
}

bool HasRoughnessMap() {
    return ((TextureFlags & HAS_ROUGHNESS_MAP) > 0u);
}

bool HasMetallicMap() {
    return ((TextureFlags & HAS_METALLIC_MAP) > 0u);
}

bool HasSheenMap() {
    return ((TextureFlags & HAS_SHEEN_MAP) > 0u);
}

bool HasEmissiveMap() {
    return ((TextureFlags & HAS_EMISSIVE_MAP) > 0u);
}