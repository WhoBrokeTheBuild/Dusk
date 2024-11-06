#include <Dusk/Material.hpp>
#include <Dusk/Graphics.hpp>

namespace dusk {

Material::Material()
{
    _shaderMaterialBuffer.reset(new VulkanBuffer());
    _shaderMaterialBuffer->Create(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        sizeof(ShaderMaterial)
    );

    _shaderMaterial.BaseColorFactor = vec4(1.0f);
    _shaderMaterial.EmissiveFactor = vec3(0.0f);
    _shaderMaterial.MetallicFactor = 1.0f;
    _shaderMaterial.NormalScale = 1.0f;
    _shaderMaterial.OcclusionStrength = 1.0f;
    _shaderMaterial.RoughnessFactor = 1.0f;
    updateShaderMaterialBuffer();
}

VulkanTexture * Material::GetBaseColorMap() const
{
    if (_baseColorMap) {
        return _baseColorMap.get();
    }
    return Graphics::WhiteTexture.get();
}

VulkanTexture * Material::GetNormalMap() const
{
    if (_normalMap) {
        return _normalMap.get();
    }
    return Graphics::WhiteTexture.get();
}

VulkanTexture * Material::GetMetallicRoughnessMap() const
{
    if (_metallicRoughnessMap) {
        return _metallicRoughnessMap.get();
    }
    return Graphics::WhiteTexture.get();
}

VulkanTexture * Material::GetEmissiveMap() const
{
    if (_emissiveMap) {
        return _emissiveMap.get();
    }
    return Graphics::BlackTexture.get();
}

VulkanTexture * Material::GetOcclusionMap() const
{
    if (_occlusionMap) {
        return _occlusionMap.get();
    }
    return Graphics::WhiteTexture.get();
}

} // namespace dusk