#include "dusk/scene/Model.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/scene/Camera.hpp>

namespace dusk {

Model::Model()
    : Actor()
{
}

void Model::Serialize(nlohmann::json& data)
{
    Actor::Serialize(data);

    const std::vector<Mesh*>& meshes = GetMeshes();
    std::vector<std::string> meshFilenames;
    for (const auto& mesh : meshes) {
        meshFilenames.push_back(mesh->GetFilename());
    }
    data["Meshes"] = meshFilenames;
}

void Model::Deserialize(nlohmann::json& data)
{
    Actor::Deserialize(data);

    if (data.find("Meshes") != data.end()) {
        std::vector<std::string> meshFilenames = data["Meshes"];
        for (auto& filename : meshFilenames) {
            AddMesh(std::make_shared<Mesh>(filename));
        }
    }
}

void Model::AddMesh(std::shared_ptr<Mesh> mesh)
{
    _meshes.push_back(mesh);
    if (_meshes.size() == 1)
    {
        _bounds = mesh->GetBounds();
    }
    else
    {
        _bounds += mesh->GetBounds();
    }
}

std::vector<Mesh*> Model::GetMeshes()
{
    std::vector<Mesh*> meshes;
    for (auto& ptr : _meshes) {
        meshes.push_back(ptr.get());
    }
    return meshes;
}

void Model::Update(UpdateContext& ctx)
{
    Actor::Update(ctx);

    _shaderData.Model = GetTransform();
}

void Model::Render(RenderContext& ctx)
{
    if (!ctx.CurrentShader || !ctx.CurrentCamera) return;

    Actor::Render(ctx);

    _shaderData.View = ctx.CurrentCamera->GetView();
    _shaderData.Proj = ctx.CurrentCamera->GetProjection();
    _shaderData.MVP = _shaderData.Proj * _shaderData.View * _shaderData.Model;

    Shader::SetUniformBufferData("DuskTransformData", &_shaderData);

    for (auto& mesh : _meshes)
    {
        mesh->Render(ctx);
    }
}

} // namespace dusk
