#ifndef DUSK_MATERIAL_HPP
#define DUSK_MATERIAL_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/asset/Texture.hpp>
#include <dusk/core/Math.hpp>

#include <memory>

namespace dusk {

class Material 
{
public:

    enum TextureID : GLint
    {
        BaseColor = 0,
        Normal,
        MetallicRoughness,
        Emissive,
        Occlusion,
    };

    enum TextureFlags : GLuint 
    {
        HasBaseColorMap             = 1 << 0,
        HasNormalMap                = 1 << 1,
        HasMetallicRoughnessMap     = 1 << 2,
        HasEmissiveMap              = 1 << 3,
        HasOcclusionMap             = 1 << 4,
    };
    
    glm::vec4 BaseColorFactor       = glm::vec4(1.0f);
    glm::vec3 EmissiveFactor        = glm::vec3(1.0f);
    
    float Metallic                  = 1.0f;
    float Roughness                 = 1.0f;
    float OcclusionStrength         = 1.0f;
    float NormalScale               = 1.0f;
    
    std::shared_ptr<Texture> BaseColorMap;
    std::shared_ptr<Texture> NormalMap;
    std::shared_ptr<Texture> MetallicRoughnessMap;
    std::shared_ptr<Texture> EmissiveMap;
    std::shared_ptr<Texture> OcclusionMap;

    Material() = default;

    virtual ~Material() = default;

    void Bind(Shader * sp);

}; // class Material

} // namespace dusk

#endif // DUSK_MATERIAL_HPP
