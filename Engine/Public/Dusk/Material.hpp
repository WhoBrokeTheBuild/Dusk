#ifndef DUSK_MATERIAL_HPP
#define DUSK_MATERIAL_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Map.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Optional.hpp>
#include <Dusk/String.hpp>
#include <Dusk/VulkanTexture.hpp>

#include <print>

#include <memory>

namespace dusk {

class DUSK_API Material
{
public:

    using Pointer = std::shared_ptr<Material>;

    static constexpr StringView BaseColorMapName         = "_BaseColorMap";
    static constexpr StringView NormalMapName            = "_NormalMap";
    static constexpr StringView MetallicRoughnessMapName = "_MetallicRoughnessMap";
    static constexpr StringView EmissiveMapName          = "_EmissiveMap";
    static constexpr StringView OcclusionMapName         = "_OcclusionMap";

    static constexpr StringView BaseColorFactorName      = "_BaseColorFactor";
    static constexpr StringView EmissiveFactorName       = "_EmissiveFactor";
    static constexpr StringView MetallicFactorName       = "_MetallicFactor";
    static constexpr StringView RoughnessFactorName      = "_RoughnessFactor";
    static constexpr StringView OcclusionStrengthName    = "_OcclusionStrength";
    static constexpr StringView NormalScaleName          = "_NormalScale";


    bool SetTexture(StringView name, VulkanTexture::Pointer texture) {
        // TODO: check name unique
        std::println("Setting {} = Texture", name);
        _textureMap[name] = texture;
        return true;
    }

    bool SetFloatParameter(StringView name, float value) {
        // TODO: check name unique
        std::println("Setting {} = {}", name, value);
        _floatMap[name] = value;
        return true;
    }

    bool SetVec3Parameter(StringView name, vec3 value) {
        // TODO: check name unique
        std::println("Setting {} = {}", name, glm::to_string(value));
        _vec3Map[name] = value;
        return true;
    }

    VulkanTexture * GetTexture(StringView name);


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

    Map<StringView, VulkanTexture::Pointer> _textureMap;

    Map<StringView, float> _floatMap;
    Map<StringView, int> _intMap;
    Map<StringView, uint> _uintMap;
    Map<StringView, vec2> _vec2Map;
    Map<StringView, vec3> _vec3Map;
    Map<StringView, vec4> _vec4Map;

}; // class Material

} // namespace dusk

#endif // DUSK_MATERIAL_HPP