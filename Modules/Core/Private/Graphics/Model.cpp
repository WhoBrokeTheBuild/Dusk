#include <Dusk/Graphics/Model.hpp>
#include <Dusk/Graphics/MeshImporter.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Log.hpp>

namespace Dusk {

void Model::Render()
{
    for (auto& mesh : _meshes) {
        mesh->Render();
    }
}

bool Model::LoadFromFile(const std::string& filename)
{
    auto gfx = GetGraphicsDriver();
    if (!gfx) {
        DuskLogError("Unable to load model '%s', no graphics driver found", filename);
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

    DuskLogError("Unable to load model '%s'", filename);
    return false;
}

} // namespace Dusk
