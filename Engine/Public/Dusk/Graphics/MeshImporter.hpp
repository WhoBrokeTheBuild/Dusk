#ifndef DUSK_CORE_MESH_IMPORTER_HPP
#define DUSK_CORE_MESH_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Graphics/Mesh.hpp>

#include <memory>
#include <vector>

namespace Dusk {

class DUSK_CORE_API IMeshImporter 
{
public:

    DISALLOW_COPY_AND_ASSIGN(IMeshImporter)

    explicit IMeshImporter() = default;

    virtual ~IMeshImporter() = default;

    virtual std::vector<std::unique_ptr<IMeshData>> LoadFromFile(const std::string& filename) = 0;

}; // class IMeshImporter

DUSK_CORE_API
void AddMeshImporter(const std::string& id, std::unique_ptr<IMeshImporter> importer);

DUSK_CORE_API
void RemoveMeshImporter(const std::string& id);

DUSK_CORE_API
const std::vector<IMeshImporter *>& GetAllMeshImporters();

} // namespace Dusk

#endif // DUSK_CORE_MESH_IMPORTER_HPP