#ifndef DUSK_MATERIAL_HPP
#define DUSK_MATERIAL_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Texture.hpp>
#include <dusk/asset/Shader.hpp>

#include <string>
#include <memory>

namespace dusk {

class Material 
{
public:

    enum TextureID {
        AMBIENT         = 0,
        DIFFUSE         = 1,
        SPECULAR        = 2,
        NORMAL          = 3,
        ALPHA           = 4,
        DISPLACEMENT    = 5,
        ROUGHNESS       = 6,
        METALLIC        = 7,
        SHEEN           = 8,
        EMISSIVE        = 9,
    };

    enum TextureFlags : uint {
        HAS_AMBIENT_MAP        = 1 << 0,
        HAS_DIFFUSE_MAP        = 1 << 1,
        HAS_SPECULAR_MAP       = 1 << 2,
        HAS_NORMAL_MAP         = 1 << 3,
        HAS_ALPHA_MAP          = 1 << 4,
        HAS_DISPLACEMENT_MAP   = 1 << 5,
        HAS_ROUGHNESS_MAP      = 1 << 6,
        HAS_METALLIC_MAP       = 1 << 7,
        HAS_SHEEN_MAP          = 1 << 8,
        HAS_EMISSIVE_MAP       = 1 << 9,
    };
    
    vec3 Ambient    = vec3(0);
    vec3 Diffuse    = vec3(0);
    vec3 Specular   = vec3(0);
    vec3 Emission   = vec3(0);
    
    float Roughness             = 0.0f;
    float Metallic              = 0.0f;
    float Shininess             = 1.0f;
    float Dissolve              = 1.0f;
    float Sheen                 = 0.0f;
    float ClearcoatThickness    = 0.0f;
    float ClearcoatRoughness    = 0.0f;
    float Anisotropy            = 0.0f;
    float AnisotropyRotation    = 0.0f;
    
    shared_ptr<Texture> AmbientMap;
    shared_ptr<Texture> DiffuseMap;
    shared_ptr<Texture> SpecularMap;
    shared_ptr<Texture> NormalMap;
    shared_ptr<Texture> AlphaMap;
    shared_ptr<Texture> DisplacementMap;
    shared_ptr<Texture> RoughnessMap;
    shared_ptr<Texture> MetallicMap;
    shared_ptr<Texture> SheenMap;
    shared_ptr<Texture> EmissiveMap;

    Material() = default;

    virtual ~Material() = default;

    void Bind(Shader * sp);

};

} // namespace dusk

#endif // DUSK_MATERIAL_HPP
