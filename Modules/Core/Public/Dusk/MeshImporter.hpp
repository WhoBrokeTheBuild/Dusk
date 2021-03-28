#ifndef DUSK_CORE_MESH_IMPORTER_HPP
#define DUSK_CORE_MESH_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Mesh.hpp>
#include <Dusk/String.hpp>

#include <memory>
#include <vector>

namespace Dusk {

class DUSK_CORE_API MeshImporter : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(MeshImporter)

    MeshImporter() = default;

    virtual ~MeshImporter() = default;

    virtual std::vector<string> GetSupportedMediaTypes() {
        return { };
    }

    virtual std::vector<std::unique_ptr<PrimitiveData>> LoadFromFile(const Path& filename, bool useAssetPath = true) = 0;

}; // class MeshImporter

DUSK_CORE_API
void AddMeshImporter(const string& id, std::unique_ptr<MeshImporter> importer);

DUSK_CORE_API
void RemoveMeshImporter(const string& id);

DUSK_CORE_API
const std::vector<MeshImporter *>& GetAllMeshImporters();

DUSK_CORE_API
const std::vector<MeshImporter *>& GetMeshImporterListForMediaType(std::string mediaType);

} // namespace Dusk

#endif // DUSK_CORE_MESH_IMPORTER_HPP