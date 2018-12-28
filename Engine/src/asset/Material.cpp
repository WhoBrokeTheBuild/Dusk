#include "dusk/asset/Material.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/asset/Shader.hpp>
#include <sstream>

namespace dusk {

Material::Material(const Data& data)
    : _ambient(data.Ambient)
    , _diffuse(data.Diffuse)
    , _specular(data.Specular)
    , _ambientMap(nullptr)
    , _diffuseMap(nullptr)
    , _specularMap(nullptr)
    , _normalMap(nullptr)
{
    if (!data.AmbientMap.empty())
    {
        _ambientMap.reset(new Texture(data.AmbientMap));
    }
    if (!data.DiffuseMap.empty())
    {
        _diffuseMap.reset(new Texture(data.DiffuseMap));
    }
    if (!data.SpecularMap.empty())
    {
        _specularMap.reset(new Texture(data.SpecularMap));
    }
    if (!data.NormalMap.empty())
    {
        _normalMap.reset(new Texture(data.NormalMap));
    }

    _shaderData.Ambient  = _ambient;
    _shaderData.Diffuse  = _diffuse;
    _shaderData.Specular = _specular;

    _shaderData.MapFlags = 0;
    _shaderData.MapFlags |= (_ambientMap && _ambientMap->IsLoaded()   ? AMBIENT_MAP_FLAG : 0);
    _shaderData.MapFlags |= (_diffuseMap && _diffuseMap->IsLoaded()   ? DIFFUSE_MAP_FLAG : 0);
    _shaderData.MapFlags |= (_specularMap && _specularMap->IsLoaded() ? SPECULAR_MAP_FLAG : 0);
    _shaderData.MapFlags |= (_normalMap && _normalMap->IsLoaded()     ? NORMAL_MAP_FLAG : 0);
}

void Material::Bind(Shader * sp)
{
    Shader::SetUniformBufferData("DuskMaterialData", &_shaderData);

    if (_ambientMap && _ambientMap->IsLoaded())
    {
        sp->SetUniform("_AmbientMap", TextureID::AMBIENT);
        glActiveTexture(GL_TEXTURE0 + TextureID::AMBIENT);
        _ambientMap->Bind();
    }

    if (_diffuseMap && _diffuseMap->IsLoaded())
    {
        sp->SetUniform("_DiffuseMap", TextureID::DIFFUSE);
        glActiveTexture(GL_TEXTURE0 + TextureID::DIFFUSE);
        _diffuseMap->Bind();
    }

    if (_specularMap && _specularMap->IsLoaded())
    {
        sp->SetUniform("_SpecularMap", TextureID::SPECULAR);
        glActiveTexture(GL_TEXTURE0 + TextureID::SPECULAR);
        _specularMap->Bind();
    }

    if (_normalMap && _normalMap->IsLoaded())
    {
        sp->SetUniform("_NormalMap", TextureID::NORMAL);
        glActiveTexture(GL_TEXTURE0 + TextureID::NORMAL);
        _normalMap->Bind();
    }

    glActiveTexture(0);
}

} // namespace dusk
