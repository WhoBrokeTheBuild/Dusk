vec3 u_Ambient;
vec3 u_Diffuse;
vec3 u_Specular;
vec3 u_Emission;

float u_Roughness;
float u_Metallic;
float u_Shininess;
float u_Dissolve;
float u_Sheen;
float u_ClearcoatThickness;
float u_ClearcoatRoughness;
float u_Anisotropy;
float u_AnisotropyRotation;

uint u_TextureFlags;

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

uniform sampler u_AmbientMap;
uniform sampler u_DiffuseMap;
uniform sampler u_SpecularMap;
uniform sampler u_NormalMap;
uniform sampler u_AlphaMap;
uniform sampler u_DisplacementMap;
uniform sampler u_RoughnessMap;
uniform sampler u_MetallicMap;
uniform sampler u_SheenMap;
uniform sampler u_EmissiveMap;

bool HasAmbientMap() {
    return ((u_TextureFlags & HAS_AMBIENT_MAP) > 0u);
}

bool HasDiffuseMap() {
    return ((u_TextureFlags & HAS_DIFFUSE_MAP) > 0u);
}

bool HasSpecularMap() {
    return ((u_TextureFlags & HAS_SPECULAR_MAP) > 0u);
}

bool HasNormalMap() {
    return ((u_TextureFlags & HAS_NORMAL_MAP) > 0u);
}

bool HasAlphaMap() {
    return ((u_TextureFlags & HAS_ALPHA_MAP) > 0u);
}

bool HasDisplacementMap() {
    return ((u_TextureFlags & HAS_DISPLACEMENT_MAP) > 0u);
}

bool HasRoughnessMap() {
    return ((u_TextureFlags & HAS_ROUGHNESS_MAP) > 0u);
}

bool HasMetallicMap() {
    return ((u_TextureFlags & HAS_METALLIC_MAP) > 0u);
}

bool HasSheenMap() {
    return ((u_TextureFlags & HAS_SHEEN_MAP) > 0u);
}

bool HasEmissiveMap() {
    return ((u_TextureFlags & HAS_EMISSIVE_MAP) > 0u);
}