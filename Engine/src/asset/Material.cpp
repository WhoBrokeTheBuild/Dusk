#include "dusk/asset/Material.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>

#include <sstream>

namespace dusk {

void Material::Bind(Shader * sp) {
    sp->Bind();

    sp->SetUniform("u_BaseColorFactor", BaseColorFactor);
    sp->SetUniform("u_EmissiveFactor", EmissiveFactor);
    sp->SetUniform("u_MetallicFactor", MetallicFactor);
    sp->SetUniform("u_RoughnessFactor", RoughnessFactor);
    sp->SetUniform("u_OcclusionStrength", OcclusionStrength);
    sp->SetUniform("u_NormalScale", NormalScale);

    unsigned int flags = 0;
    
    if (BaseColorMap) {
        flags |= HasBaseColorMap;
        sp->SetUniform("u_BaseColorMap", TextureID::BaseColor);
        glActiveTexture(GL_TEXTURE0 + TextureID::BaseColor);
        BaseColorMap->Bind();
    }
    
    if (NormalMap) {
        flags |= HasNormalMap;
        sp->SetUniform("u_NormalMap", TextureID::Normal);
        glActiveTexture(GL_TEXTURE0 + TextureID::Normal);
        NormalMap->Bind();
    }
    
    if (MetallicRoughnessMap) {
        flags |= HasMetallicRoughnessMap;
        sp->SetUniform("u_MetallicRoughnessMap", TextureID::MetallicRoughness);
        glActiveTexture(GL_TEXTURE0 + TextureID::MetallicRoughness);
        MetallicRoughnessMap->Bind();
    }
    
    if (EmissiveMap) {
        flags |= HasEmissiveMap;
        sp->SetUniform("u_EmissiveMap", TextureID::Emissive);
        glActiveTexture(GL_TEXTURE0 + TextureID::Emissive);
        EmissiveMap->Bind();
    }
    
    if (OcclusionMap) {
        flags |= HasOcclusionMap;
        sp->SetUniform("u_OcclusionMap", TextureID::Occlusion);
        glActiveTexture(GL_TEXTURE0 + TextureID::Occlusion);
        OcclusionMap->Bind();
    }

    sp->SetUniform("u_TextureFlags", flags);
}

} // namespace dusk
