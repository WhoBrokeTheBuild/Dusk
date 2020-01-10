#include <Dusk/Graphics/MeshComponent.hpp>
#include <Dusk/Graphics/MeshImporter.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Log.hpp>

namespace Dusk {

MeshComponent::MeshComponent(std::unique_ptr<IMesh> && mesh)
{
    AddMesh(std::move(mesh));
}

MeshComponent::MeshComponent(std::vector<std::unique_ptr<IMesh>> && meshes)
{
    AddMeshes(std::move(meshes));
}

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

void MeshComponent::AddMesh(std::unique_ptr<IMesh> && mesh)
{
    _meshes.push_back(std::move(mesh));
}

void MeshComponent::AddMeshes(std::vector<std::unique_ptr<IMesh>> && meshes)
{
    for (auto&& mesh : meshes) {
        _meshes.push_back(std::move(mesh));
    }
}

void MeshComponent::Render(RenderContext * ctx)
{
    for (auto& mesh : _meshes) {
        mesh->Render();
    }
}

} // namespace Dusk
