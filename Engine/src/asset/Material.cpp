#include "dusk/asset/Material.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/asset/Shader.hpp>
#include <sstream>

namespace dusk {

void Material::Bind(Shader * sp) {
    sp->SetUniform("Ambient", Ambient);
    sp->SetUniform("Diffuse", Diffuse);
    sp->SetUniform("Specular", Specular);
    sp->SetUniform("Emission", Emission);
    sp->SetUniform("Roughness", Roughness);
    sp->SetUniform("Metallic", Metallic);
    sp->SetUniform("Shininess", Shininess);
    sp->SetUniform("Dissolve", Dissolve);
    sp->SetUniform("Sheen", Sheen);
    sp->SetUniform("ClearcoatThickness", ClearcoatThickness);
    sp->SetUniform("ClearcoatRoughness", ClearcoatRoughness);
    sp->SetUniform("Anisotropy", Anisotropy);
    sp->SetUniform("AnisotropyRotation", AnisotropyRotation);

    uint flags = 0;
    
    if (AmbientMap != nullptr) {
        flags |= HAS_AMBIENT_MAP;
        sp->SetUniform("AmbientMap", AmbientMap->GetGLId());
    }
    
    if (DiffuseMap != nullptr) {
        flags |= HAS_DIFFUSE_MAP;
        sp->SetUniform("DiffuseMap", DiffuseMap->GetGLId());
    }
    
    if (SpecularMap != nullptr) {
        flags |= HAS_SPECULAR_MAP;
        sp->SetUniform("SpecularMap", SpecularMap->GetGLId());
    }
    
    if (NormalMap != nullptr) {
        flags |= HAS_NORMAL_MAP;
        sp->SetUniform("NormalMap", NormalMap->GetGLId());
    }
    
    if (AlphaMap != nullptr) {
        flags |= HAS_ALPHA_MAP;
        sp->SetUniform("AlphaMap", AlphaMap->GetGLId());
    }
    
    if (DisplacementMap != nullptr) {
        flags |= HAS_DISPLACEMENT_MAP;
        sp->SetUniform("(DisplacementMap", (DisplacementMap->GetGLId());
    }
    
    if (RoughnessMap != nullptr) {
        flags |= HAS_ROUGHNESS_MAP;
        sp->SetUniform("RoughnessMap", RoughnessMap->GetGLId());
    }
    
    if (MetallicMap != nullptr) {
        flags |= HAS_METALLIC_MAP;
        sp->SetUniform("MetallicMap", MetallicMap->GetGLId());
    }
    
    if (SheenMap != nullptr) {
        flags |= HAS_SHEEN_MAP;
        sp->SetUniform("SheenMap", SheenMap->GetGLId());
    }
    
    if (EmissiveMap != nullptr) {
        flags |= HAS_EMISSIVE_MAP;
        sp->SetUniform("EmissiveMap", EmissiveMap->GetGLId());
    }

    sp->SetUniform("TextureFlags", flags);
}

} // namespace dusk
