#ifndef DUSK_MATERIAL_HPP
#define DUSK_MATERIAL_HPP

#include <dusk/asset/Texture.hpp>
#include <dusk/core/Math.hpp>

#include <memory>
#include <unordered_map>
#include <string>

namespace dusk {

class Shader;

class Material
{
public:

    enum TextureID : GLint
    {
        BASE_COLOR = 0,
        NORMAL,
        METALLIC_ROUGHNESS,
        EMISSIVE,
        OCCLUSION,
    };
    
    glm::vec4 BaseColorFactor = glm::vec4(1.0f);
    glm::vec3 EmissiveFactor  = glm::vec3(1.0f);
    
    float MetallicFactor    = 1.0f;
    float RoughnessFactor   = 1.0f;
    float OcclusionStrength = 1.0f;
    float NormalScale       = 1.0f;
    
    std::shared_ptr<Texture> BaseColorMap;
    std::shared_ptr<Texture> NormalMap;
    std::shared_ptr<Texture> MetallicRoughnessMap;
    std::shared_ptr<Texture> EmissiveMap;
    std::shared_ptr<Texture> OcclusionMap;

    std::unordered_map<std::string, std::string> GetDefines() const;

    Material() = default;

    virtual ~Material() = default;

    void Bind(Shader *);

}; // class Material

} // namespace dusk

#endif // DUSK_MATERIAL_HPP
