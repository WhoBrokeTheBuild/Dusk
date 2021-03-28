#ifndef DUSK_MATERIAL_HPP
#define DUSK_MATERIAL_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Buffer.hpp>
#include <Dusk/Texture.hpp>
#include <Dusk/ShaderMaterial.hpp>

namespace Dusk {

class DUSK_CORE_API Material : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Material);

    Material() = default;

    virtual ~Material() = default;

    virtual void Initialize();

    virtual void Terminate();

    virtual inline vec4 GetBaseColorFactor() const {
        return _shaderMaterial.BaseColorFactor;
    }

    virtual inline vec3 GetEmissiveFactor() const {
        return _shaderMaterial.EmissiveFactor;
    }

    virtual inline float GetMetallicFactor() const {
        return _shaderMaterial.MetallicFactor;
    }

    virtual inline float GetRoughnessFactor() const {
        return _shaderMaterial.RoughnessFactor;
    }

    virtual inline float GetOcclusionStrength() const {
        return _shaderMaterial.OcclusionStrength;
    }

    virtual inline float GetNormalScale() const {
        return _shaderMaterial.NormalScale;
    }

    virtual inline void SetBaseColorFactor(vec4 factor) {
        _shaderMaterial.BaseColorFactor = factor;
        UpdateBuffer();
    }

    virtual inline void SetEmissiveFactor(vec3 factor) {
        _shaderMaterial.EmissiveFactor = factor;
        UpdateBuffer();
    }

    virtual inline void SetMetallicFactor(float factor) {
        _shaderMaterial.MetallicFactor = factor;
        UpdateBuffer();
    }

    virtual inline void SetRoughnessFactor(float factor) {
        _shaderMaterial.RoughnessFactor = factor;
        UpdateBuffer();
    }

    virtual inline void SetOcclusionStrength(float strength) {
        _shaderMaterial.OcclusionStrength = strength;
        UpdateBuffer();
    }

    virtual inline void SetNormalScale(float scale) {
        _shaderMaterial.NormalScale = scale;
        UpdateBuffer();
    }

    virtual inline void SetBaseColorMap(std::shared_ptr<Texture> map) {
        _baseColorMap = map;
    }

    virtual inline void SetNormalMap(std::shared_ptr<Texture> map) {
        _normalMap = map;
    }

    virtual inline void SetMetallicRoughnessMap(std::shared_ptr<Texture> map) {
        _metallicRoughnessMap = map;
    }

    virtual inline void SetEmissiveMap(std::shared_ptr<Texture> map) {
        _emissiveMap = map;
    }

    virtual inline void SetOcclusionMap(std::shared_ptr<Texture> map) {
        _occlusionMap = map;
    }

    virtual inline std::shared_ptr<Texture> GetBaseColorMap() {
        return _baseColorMap;
    }

    virtual inline std::shared_ptr<Texture> GetNormalMap() {
        return _normalMap;
    }

    virtual inline std::shared_ptr<Texture> GetMetallicRoughnessMap() {
        return _metallicRoughnessMap;
    }

    virtual inline std::shared_ptr<Texture> GetEmissiveMap() {
        return _emissiveMap;
    }

    virtual inline std::shared_ptr<Texture> GetOcclusionMap() {
        return _occlusionMap;
    }

    // TODO: Fix buffer thrashing when setting multiple variables
    virtual void UpdateBuffer();

protected:

    std::shared_ptr<Buffer> _shaderMaterialBuffer;

    ShaderMaterial _shaderMaterial;

    std::shared_ptr<Texture> _baseColorMap;

    std::shared_ptr<Texture> _normalMap;

    std::shared_ptr<Texture> _metallicRoughnessMap;

    std::shared_ptr<Texture> _emissiveMap;

    std::shared_ptr<Texture> _occlusionMap;

}; // class Material

}; // namespace Dusk

#endif // DUSK_MATERIAL_HPP