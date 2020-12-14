#include <Dusk/Mesh.hpp>
#include <Dusk/MeshImporter.hpp>
#include <Dusk/GraphicsDriver.hpp>

namespace Dusk {

std::vector<std::shared_ptr<Mesh>> LoadMeshListFromFile(const std::string& filename)
{
    GraphicsDriver * gfx = GetGraphicsDriver();

    const auto& importers = GetAllMeshImporters();
    for (const auto& importer : importers) {
        const auto& meshDataList = importer->LoadFromFile(filename);
        if (meshDataList.empty()) {
            continue;
        }

        std::vector<std::shared_ptr<Mesh>> meshList;
        meshList.reserve(meshDataList.size());

        for (const auto& meshData : meshDataList) {
            auto mesh = gfx->CreateMesh();
            
            if (!mesh->Load(meshData.get())) {
                break;
            }

            meshList.push_back(mesh);
        }

        return meshList;
    }

    DuskLogError("Failed to load mesh '%s'", filename);
    return { };
}

} // namespace Dusk