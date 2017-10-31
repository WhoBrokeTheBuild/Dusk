#include "dusk/scene/Model.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/scene/Camera.hpp>

namespace dusk {

Model::Model(std::string name, Actor * parent /*= nullptr*/)
    : Actor(name, parent)
{
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

void Model::Update(const UpdateContext& ctx)
{
    _shaderData.Model = GetTransform();
}

void Model::Render(RenderContext& ctx)
{
    if (!ctx.CurrentShader || !ctx.CurrentCamera) return;

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
