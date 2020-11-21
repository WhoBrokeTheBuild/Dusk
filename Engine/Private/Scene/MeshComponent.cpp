#include <Dusk/Scene/MeshComponent.hpp>
#include <Dusk/Graphics/MeshImporter.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Scene/Entity.hpp>
#include <Dusk/Log.hpp>

namespace Dusk {

bool MeshComponent::LoadFromFile(const std::string& filename)
{
    auto gfx = GetGraphicsDriver();
    if (!gfx) {
        DuskLogError("Unable to load mesh '%s', no graphics driver found", filename);
        return false;
    }

    const auto& importers = GetAllMeshImporters();
    for (const auto& importer : importers) {
        auto meshes = importer->LoadFromFile(filename);
        if (!meshes.empty()) {
            for (auto& meshData : meshes) {
                auto mesh = gfx->CreateMesh();
                if (mesh->Load(meshData.get())) {
                    _meshes.push_back(std::move(mesh));
                }
            }
            return true;
        }
    }

    DuskLogError("Unable to load mesh '%s'", filename);
    return false;
}

void MeshComponent::AddMesh(std::shared_ptr<Mesh> && mesh)
{
    _meshes.push_back(std::move(mesh));
}

void MeshComponent::AddMeshes(std::vector<std::shared_ptr<Mesh>> && meshes)
{
    for (auto&& mesh : meshes) {
        _meshes.push_back(std::move(mesh));
    }
}

void MeshComponent::Render(RenderContext * ctx)
{
    auto gfx = GetGraphicsDriver();
    auto transformData = ctx->GetTransformData();

    transformData->Model = GetEntity()->GetWorldTransform();
    transformData->UpdateMVP();
    
    gfx->SetShaderData("DuskTransformData", sizeof(TransformData), transformData);

    for (auto& mesh : _meshes) {
        mesh->Render();
    }
}

} // namespace Dusk
