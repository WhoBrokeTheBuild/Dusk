#include <Dusk/Mesh.hpp>
#include <Dusk/MeshImporter.hpp>
#include <Dusk/GraphicsDriver.hpp>

namespace Dusk {

std::shared_ptr<Mesh> LoadMeshFromFile(const std::string& filename)
{
    GraphicsDriver * gfx = GetGraphicsDriver();

    const auto& importers = GetAllMeshImporters();
    for (const auto& importer : importers) {
        const auto& primitiveList = importer->LoadFromFile(filename);
        if (primitiveList.empty()) {
            continue;
        }

        auto mesh = gfx->CreateMesh();
        if (!mesh->Load(primitiveList)) {
            break;
        }

        return mesh;
    }

    DuskLogError("Failed to load mesh '%s'", filename);
    return nullptr;
}

} // namespace Dusk