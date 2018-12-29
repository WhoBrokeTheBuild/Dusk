#ifndef DUSK_MATERIAL_HPP
#define DUSK_MATERIAL_HPP

#include <dusk/Config.hpp>

#include <dusk/asset/Texture.hpp>
#include <dusk/asset/Shader.hpp>

using glm::vec4;
using glm::vec3;

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

namespace dusk {

struct MaterialData
{
    vec3 Ambient             = vec3(0, 0, 0);
    float Roughness          = 0.0f;
    vec3 Diffuse             = vec3(0, 0, 0);
    float Metallic           = 0.0f;
    vec3 Specular            = vec3(0, 0, 0);
    float SpecularExponent   = 1.0f;
    vec3 Emission            = vec3(0, 0, 0);
    float Dissolve           = 1.0f;
    float Sheen              = 0.0f;
    float ClearcoatThickness = 0.0f;
    float ClearcoatRoughness = 0.0f;
    float Anisotropy         = 0.0f;
    float AnisotropyRotation = 0.0f;
    GLuint MapFlags          = 0;
};

class Material 
{
public:

    enum TextureID
    {
        AMBIENT  = 0,
        DIFFUSE  = 1,
        SPECULAR = 2,
        NORMAL   = 3,
    };

    enum MapFlags : GLuint
    {
        AMBIENT_MAP_FLAG  = 1,
        DIFFUSE_MAP_FLAG  = 2,
        SPECULAR_MAP_FLAG = 4,
        NORMAL_MAP_FLAG   = 8,
    };

    struct Data
    {
        vec3 Ambient = vec3(0);
        vec3 Diffuse = vec3(0);
        vec3 Specular = vec3(0);

        string AmbientMap = "";
        string DiffuseMap = "";
        string SpecularMap = "";
        string NormalMap = "";
    };

    DISALLOW_COPY_AND_ASSIGN(Material)

    Material(const Data& data);

    virtual ~Material() = default;

    void Bind(Shader * sp);

private:

    MaterialData _shaderData;

    vec3 _ambient;
    vec3 _diffuse;
    vec3 _specular;

    shared_ptr<Texture> _ambientMap;
    shared_ptr<Texture> _diffuseMap;
    shared_ptr<Texture> _specularMap;
    shared_ptr<Texture> _normalMap;

};

} // namespace dusk

#endif // DUSK_MATERIAL_HPP
