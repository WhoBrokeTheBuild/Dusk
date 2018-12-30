#include "dusk/asset/Material.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/asset/Shader.hpp>
#include <sstream>

namespace dusk {

void Material::Bind(Shader * sp) {
    sp->SetUniform("u_Ambient", Ambient);
    sp->SetUniform("u_Diffuse", Diffuse);
    sp->SetUniform("u_Specular", Specular);
    sp->SetUniform("u_Emission", Emission);
    sp->SetUniform("u_Roughness", Roughness);
    sp->SetUniform("u_Metallic", Metallic);
    sp->SetUniform("u_Shininess", Shininess);
    sp->SetUniform("u_Dissolve", Dissolve);
    sp->SetUniform("u_Sheen", Sheen);
    sp->SetUniform("u_ClearcoatThickness", ClearcoatThickness);
    sp->SetUniform("u_ClearcoatRoughness", ClearcoatRoughness);
    sp->SetUniform("u_Anisotropy", Anisotropy);
    sp->SetUniform("u_AnisotropyRotation", AnisotropyRotation);

    uint flags = 0;
    
    if (AmbientMap) {
        flags |= HAS_AMBIENT_MAP;
        sp->SetUniform("u_AmbientMap", TextureID::AMBIENT);
        glActiveTexture(GL_TEXTURE0 + TextureID::AMBIENT);
        AmbientMap->Bind();
    }
    
    if (DiffuseMap) {
        flags |= HAS_DIFFUSE_MAP;
        sp->SetUniform("u_DiffuseMap", TextureID::DIFFUSE);
        glActiveTexture(GL_TEXTURE0 + TextureID::DIFFUSE);
        DiffuseMap->Bind();
    }
    
    if (SpecularMap) {
        flags |= HAS_SPECULAR_MAP;
        sp->SetUniform("u_SpecularMap", TextureID::SPECULAR);
        glActiveTexture(GL_TEXTURE0 + TextureID::SPECULAR);
        SpecularMap->Bind();
    }
    
    if (NormalMap) {
        flags |= HAS_NORMAL_MAP;
        sp->SetUniform("u_NormalMap", TextureID::NORMAL);
        glActiveTexture(GL_TEXTURE0 + TextureID::NORMAL);
        NormalMap->Bind();
    }
    
    if (AlphaMap) {
        flags |= HAS_ALPHA_MAP;
        sp->SetUniform("u_AlphaMap", TextureID::ALPHA);
        glActiveTexture(GL_TEXTURE0 + TextureID::ALPHA);
        AlphaMap->Bind();
    }
    
    if (DisplacementMap) {
        flags |= HAS_DISPLACEMENT_MAP;
        sp->SetUniform("u_DisplacementMap",TextureID::DISPLACEMENT);
        glActiveTexture(GL_TEXTURE0 + TextureID::DISPLACEMENT);
        DisplacementMap->Bind();
    }
    
    if (RoughnessMap) {
        flags |= HAS_ROUGHNESS_MAP;
        sp->SetUniform("u_RoughnessMap", TextureID::ROUGHNESS);
        glActiveTexture(GL_TEXTURE0 + TextureID::ROUGHNESS);
        RoughnessMap->Bind();
    }
    
    if (MetallicMap) {
        flags |= HAS_METALLIC_MAP;
        sp->SetUniform("u_MetallicMap", TextureID::METALLIC);
        glActiveTexture(GL_TEXTURE0 + TextureID::METALLIC);
        MetallicMap->Bind();
    }
    
    if (SheenMap) {
        flags |= HAS_SHEEN_MAP;
        sp->SetUniform("u_SheenMap", TextureID::SHEEN);
        glActiveTexture(GL_TEXTURE0 + TextureID::SHEEN);
        SheenMap->Bind();
    }
    
    if (EmissiveMap) {
        flags |= HAS_EMISSIVE_MAP;
        sp->SetUniform("u_EmissiveMap", TextureID::EMISSIVE);
        glActiveTexture(GL_TEXTURE0 + TextureID::EMISSIVE);
        EmissiveMap->Bind();
    }

    sp->SetUniform("u_TextureFlags", flags);
}

} // namespace dusk
