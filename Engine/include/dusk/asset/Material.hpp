#ifndef DUSK_MATERIAL_HPP
#define DUSK_MATERIAL_HPP

#include <dusk/Config.hpp>

#include <dusk/asset/Texture.hpp>
#include <dusk/asset/Shader.hpp>

using glm::vec4;

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

namespace dusk {

struct MaterialData
{
    alignas(16) vec4 Ambient  = vec4(0, 0, 0, 1);
    alignas(16) vec4 Diffuse  = vec4(0, 0, 0, 1);
    alignas(16) vec4 Specular = vec4(0, 0, 0, 1);

    alignas(4) GLuint MapFlags = 0;
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
        vec4 Ambient = vec4(0);
        vec4 Diffuse = vec4(0);
        vec4 Specular = vec4(0);

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

    vec4 _ambient;
    vec4 _diffuse;
    vec4 _specular;

    shared_ptr<Texture> _ambientMap;
    shared_ptr<Texture> _diffuseMap;
    shared_ptr<Texture> _specularMap;
    shared_ptr<Texture> _normalMap;

};

} // namespace dusk

#endif // DUSK_MATERIAL_HPP
