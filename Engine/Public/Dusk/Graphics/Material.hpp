#ifndef DUSK_MATERIAL_HPP
#define DUSK_MATERIAL_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Graphics/Texture.hpp>

namespace Dusk {

class DUSK_ENGINE_API Material 
{
public:

    // enum TextureID : int
    // {
    //     BASE_COLOR = 0,
    //     NORMAL,
    //     METALLIC_ROUGHNESS,
    //     EMISSIVE,
    //     OCCLUSION,
    // };

    // glm::vec4 BaseColorFactor = glm::vec4(1.0f);
    // glm::vec3 EmissiveFactor  = glm::vec3(1.0f);
    
    // float MetallicFactor    = 1.0f;
    // float RoughnessFactor   = 1.0f;
    // float OcclusionStrength = 1.0f;
    // float NormalScale       = 1.0f;

    // std::shared_ptr<Texture> BaseColorMap;
    // std::shared_ptr<Texture> NormalMap;
    // std::shared_ptr<Texture> MetallicRoughnessMap;
    // std::shared_ptr<Texture> EmissiveMap;
    // std::shared_ptr<Texture> OcclusionMap;

    // std::unordered_map<std::string, std::string> GetDefines() const;
    
private:

};

}; // namespace Dusk

#endif // DUSK_MATERIAL_HPP