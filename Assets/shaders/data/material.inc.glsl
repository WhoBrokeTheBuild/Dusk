#ifndef MATERIAL_INC
#define MATERIAL_INC

uniform vec4 u_BaseColorFactor;

uniform vec3 u_EmissiveFactor;

uniform float u_MetallicFactor;
uniform float u_RoughnessFactor;
uniform float u_OcclusionStrength;
uniform float u_NormalScale;

uniform uint u_TextureFlags;

const uint c_HasBaseColorMap			= 1 << 0;
const uint c_HasNormalMap			    = 1 << 1;
const uint c_HasMetallicRoughnessMap	= 1 << 2;
const uint c_HasEmissiveMap				= 1 << 3;
const uint c_HasOcclusionMap			= 1 << 4;

uniform sampler2D u_BaseColorMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_MetallicRoughnessMap;
uniform sampler2D u_EmissiveMap;
uniform sampler2D u_OcclusionMap;

bool HasBaseColorMap() {
    return ((u_TextureFlags & c_HasBaseColorMap) > 0u);
}

bool HasNormalMap() {
    return ((u_TextureFlags & c_HasNormalMap) > 0u);
}

bool HasMetallicRoughnessMap() {
    return ((u_TextureFlags & c_HasMetallicRoughnessMap) > 0u);
}

bool HasEmissiveMap() {
    return ((u_TextureFlags & c_HasEmissiveMap) > 0u);
}

bool HasOcclusionMap() {
    return ((u_TextureFlags & c_HasOcclusionMap) > 0u);
}

#endif // MATERIAL_INC