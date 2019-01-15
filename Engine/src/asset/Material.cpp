#include "dusk/asset/Material.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>

#include <sstream>

namespace dusk {

std::unordered_map<std::string, std::string> Material::GetDefines() const
{
    std::unordered_map<std::string, std::string> defines;
    
    if (BaseColorMap) {
        defines["HAS_BASE_COLOR_MAP"] = std::string();
    }
    
    if (NormalMap) {
        defines["HAS_NORMAL_MAP"] = std::string();
    }
    
    if (MetallicRoughnessMap) {
        defines["HAS_METALLIC_ROUGHNESS_MAP"] = std::string();
    }
    
    if (EmissiveMap) {
        defines["HAS_EMISSIVE_MAP"] = std::string();
    }
    
    if (OcclusionMap) {
        defines["HAS_OCCLUSION_MAP"] = std::string();
    }

    return defines;
}

void Material::Bind(Shader * sp) 
{
    sp->Bind();

    sp->SetUniform("u_BaseColorFactor", BaseColorFactor);
    sp->SetUniform("u_EmissiveFactor", EmissiveFactor);
    sp->SetUniform("u_MetallicFactor", MetallicFactor);
    sp->SetUniform("u_RoughnessFactor", RoughnessFactor);
    sp->SetUniform("u_OcclusionStrength", OcclusionStrength);
    sp->SetUniform("u_NormalScale", NormalScale);

    if (BaseColorMap) {
        sp->SetUniform("u_BaseColorMap", TextureID::BASE_COLOR);
        glActiveTexture(GL_TEXTURE0 + TextureID::BASE_COLOR);
        BaseColorMap->Bind();
    }
    
    if (NormalMap) {
        sp->SetUniform("u_NormalMap", TextureID::NORMAL);
        glActiveTexture(GL_TEXTURE0 + TextureID::NORMAL);
        NormalMap->Bind();
    }
    
    if (MetallicRoughnessMap) {
        sp->SetUniform("u_MetallicRoughnessMap", TextureID::METALLIC_ROUGHNESS);
        glActiveTexture(GL_TEXTURE0 + TextureID::METALLIC_ROUGHNESS);
        MetallicRoughnessMap->Bind();
    }
    
    if (EmissiveMap) {
        sp->SetUniform("u_EmissiveMap", TextureID::EMISSIVE);
        glActiveTexture(GL_TEXTURE0 + TextureID::EMISSIVE);
        EmissiveMap->Bind();
    }
    
    if (OcclusionMap) {
        sp->SetUniform("u_OcclusionMap", TextureID::OCCLUSION);
        glActiveTexture(GL_TEXTURE0 + TextureID::OCCLUSION);
        OcclusionMap->Bind();
    }
}

} // namespace dusk
