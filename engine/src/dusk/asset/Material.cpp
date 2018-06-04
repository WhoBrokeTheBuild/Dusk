#include "dusk/asset/Material.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/asset/AssetLoader.hpp>
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
        _ambientMap = AssetLoader::LoadAndLock<Texture>(data.AmbientMap);
    }
    if (!data.DiffuseMap.empty())
    {
        _diffuseMap = AssetLoader::LoadAndLock<Texture>(data.DiffuseMap);
    }
    if (!data.SpecularMap.empty())
    {
        _specularMap = AssetLoader::LoadAndLock<Texture>(data.SpecularMap);
    }
    if (!data.NormalMap.empty())
    {
        _normalMap = AssetLoader::LoadAndLock<Texture>(data.NormalMap);
    }

    _shaderData.Ambient  = _ambient;
    _shaderData.Diffuse  = _diffuse;
    _shaderData.Specular = _specular;

    _shaderData.MapFlags = 0;
    _shaderData.MapFlags |= (_ambientMap  ? AMBIENT_MAP_FLAG : 0);
    _shaderData.MapFlags |= (_diffuseMap  ? DIFFUSE_MAP_FLAG : 0);
    _shaderData.MapFlags |= (_specularMap ? SPECULAR_MAP_FLAG : 0);
    _shaderData.MapFlags |= (_normalMap   ? NORMAL_MAP_FLAG : 0);
}

void Material::Bind(Shader * sp)
{
    App * app = App::Inst();
    Shader::SetUniformBufferData("DuskMaterialData", &_shaderData);

    if (_normalMap) {
        sp->SetUniform("_NormalMap", TextureID::NORMAL);
        glActiveTexture(GL_TEXTURE0 + TextureID::NORMAL);
        _normalMap->Bind();
    }
    else {
        sp->SetUniform("_NormalMap", -1);
    }

    if (_ambientMap) {
        sp->SetUniform("_AmbientMap", TextureID::AMBIENT);
        glActiveTexture(GL_TEXTURE0 + TextureID::AMBIENT);
        _ambientMap->Bind();
    }
    else {
        sp->SetUniform("_AmbientMap", -1);
    }

    if (_diffuseMap) {
        sp->SetUniform("_DiffuseMap", TextureID::DIFFUSE);
        glActiveTexture(GL_TEXTURE0 + TextureID::DIFFUSE);
        _diffuseMap->Bind();
    }
    else {
        sp->SetUniform("_DiffuseMap", -1);
    }

    if (_specularMap) {
        sp->SetUniform("_SpecularMap", TextureID::SPECULAR);
        glActiveTexture(GL_TEXTURE0 + TextureID::SPECULAR);
        _specularMap->Bind();
    }
    else {
        sp->SetUniform("_SpecularMap", -1);
    }

    glActiveTexture(0);
}

} // namespace dusk
