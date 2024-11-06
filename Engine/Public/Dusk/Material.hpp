#ifndef DUSK_MATERIAL_HPP
#define DUSK_MATERIAL_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Map.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Optional.hpp>
#include <Dusk/String.hpp>
#include <Dusk/VulkanTexture.hpp>
#include <Dusk/VulkanBuffer.hpp>
#include <Dusk/ShaderMaterial.hpp>

#include <print>

#include <memory>

namespace dusk {

class DUSK_API Material
{
public:

    using Pointer = std::shared_ptr<Material>;

    Material();

    virtual ~Material() = default;

    inline VulkanBuffer * GetBuffer() {
        return _shaderMaterialBuffer.get();
    }

    inline void SetBaseColorFactor(vec4 value) {
        _shaderMaterial.BaseColorFactor = value;
        updateShaderMaterialBuffer();
    }

    inline vec4 GetBaseColorFactor() {
        return _shaderMaterial.BaseColorFactor;
    }

    inline void SetEmissiveFactor(vec3 value) {
        _shaderMaterial.EmissiveFactor = value;
        updateShaderMaterialBuffer();
    }

    inline vec3 GetEmissiveFactor() {
        return _shaderMaterial.EmissiveFactor;
    }

    inline void SetMetallicFactor(float value) {
        _shaderMaterial.MetallicFactor = value;
        updateShaderMaterialBuffer();
    }

    inline float GetMetallicFactor() {
        return _shaderMaterial.MetallicFactor;
    }

    inline void SetRoughnessFactor(float value) {
        _shaderMaterial.RoughnessFactor = value;
        updateShaderMaterialBuffer();
    }

    inline float GetRoughnessFactor() {
        return _shaderMaterial.RoughnessFactor;
    }

    inline void SetOcclusionStrength(float value) {
        _shaderMaterial.OcclusionStrength = value;
        updateShaderMaterialBuffer();
    }

    inline float GetOcclusionStrength() {
        return _shaderMaterial.OcclusionStrength;
    }

    inline void SetNormalScale(float value) {
        _shaderMaterial.NormalScale = value;
        updateShaderMaterialBuffer();
    }

    inline float GetNormalScale() {
        return _shaderMaterial.NormalScale;
    }

    inline void SetBaseColorMap(VulkanTexture::Pointer texture) {
        _baseColorMap = texture;
    }

    VulkanTexture * GetBaseColorMap() const;

    inline void SetNormalMap(VulkanTexture::Pointer texture) {
        _normalMap = texture;
    }

    VulkanTexture * GetNormalMap() const;

    inline void SetMetallicRoughnessMap(VulkanTexture::Pointer texture) {
        _metallicRoughnessMap = texture;
    }

    VulkanTexture * GetMetallicRoughnessMap() const;

    inline void SetEmissiveMap(VulkanTexture::Pointer texture) {
        _emissiveMap = texture;
    }

    VulkanTexture * GetEmissiveMap() const;

    inline void SetOcclusionMap(VulkanTexture::Pointer texture) {
        _occlusionMap = texture;
    }

    VulkanTexture * GetOcclusionMap() const;

    // static constexpr StringView BaseColorMapName         = "_BaseColorMap";
    // static constexpr StringView NormalMapName            = "_NormalMap";
    // static constexpr StringView MetallicRoughnessMapName = "_MetallicRoughnessMap";
    // static constexpr StringView EmissiveMapName          = "_EmissiveMap";
    // static constexpr StringView OcclusionMapName         = "_OcclusionMap";

    // static constexpr StringView BaseColorFactorName      = "_BaseColorFactor";
    // static constexpr StringView EmissiveFactorName       = "_EmissiveFactor";
    // static constexpr StringView MetallicFactorName       = "_MetallicFactor";
    // static constexpr StringView RoughnessFactorName      = "_RoughnessFactor";
    // static constexpr StringView OcclusionStrengthName    = "_OcclusionStrength";
    // static constexpr StringView NormalScaleName          = "_NormalScale";

    // bool SetTexture(StringView name, VulkanTexture::Pointer texture) {
    //     // TODO: check name unique
    //     std::println("Setting {} = Texture", name);
    //     _textureMap[name] = texture;
    //     return true;
    // }

    // VulkanTexture * GetTexture(StringView name);

    // bool SetFloatParameter(StringView name, float value) {
    //     // TODO: check name unique
    //     std::println("Setting {} = {}", name, value);
    //     _floatMap[name] = value;
    //     return true;
    // }

    // bool SetVec3Parameter(StringView name, vec3 value) {
    //     // TODO: check name unique
    //     std::println("Setting {} = {}", name, glm::to_string(value));
    //     _vec3Map[name] = value;
    //     return true;
    // }

    // float GetFloatParameter(StringView name);
    // int GetIntParameter(StringView name);
    // bool SetIntParameter(StringView name, int value);
    // uint GetUIntParameter(StringView name);
    // bool SetUIntParameter(StringView name, uint value);
    // vec2 GetVec2Parameter(StringView name);
    // bool SetVec2Parameter(StringView name, vec2 value);
    // vec3 GetVec3Parameter(StringView name);
    // vec4 GetVec4Parameter(StringView name);
    // bool SetVec4Parameter(StringView name, vec4 value);
    // mat3 GetMat3Parameter(StringView name);
    // bool SetMat3Parameter(StringView name, mat3 value);
    // mat4 GetMat4Parameter(StringView name);
    // bool SetMat4Parameter(StringView name, mat4 value);

private:

    inline void updateShaderMaterialBuffer() {
        _shaderMaterialBuffer->WriteTo(0, sizeof(ShaderMaterial), &_shaderMaterial);
    }

    VulkanBuffer::Pointer _shaderMaterialBuffer;

    ShaderMaterial _shaderMaterial;
    VulkanTexture::Pointer _baseColorMap;
    VulkanTexture::Pointer _normalMap;
    VulkanTexture::Pointer _metallicRoughnessMap;
    VulkanTexture::Pointer _emissiveMap;
    VulkanTexture::Pointer _occlusionMap;

    // Map<StringView, VulkanTexture::Pointer> _textureMap;

    // Map<StringView, float> _floatMap;
    // Map<StringView, int> _intMap;
    // Map<StringView, uint> _uintMap;
    // Map<StringView, vec2> _vec2Map;
    // Map<StringView, vec3> _vec3Map;
    // Map<StringView, vec4> _vec4Map;

}; // class Material

} // namespace dusk

#endif // DUSK_MATERIAL_HPP