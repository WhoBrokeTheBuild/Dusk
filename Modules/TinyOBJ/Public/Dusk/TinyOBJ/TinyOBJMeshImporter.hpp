#ifndef DUSK_TINYOBJ_MESH_IMPORTER_HPP
#define DUSK_TINYOBJ_MESH_IMPORTER_HPP

#include <Dusk/TinyOBJ/TinyOBJConfig.hpp>

#include <Dusk/MeshImporter.hpp>

#include <vector>

namespace Dusk::TinyOBJ {

class DUSK_TINYOBJ_API TinyOBJMeshImporter : public MeshImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(TinyOBJMeshImporter)

    TinyOBJMeshImporter() = default;

    std::vector<std::unique_ptr<Dusk::MeshData>> LoadFromFile(const std::string& filename) override;

}; // class TinyOBJMeshImporter

} // namespace Dusk::TinyOBJ

#endif // DUSK_TINYOBJ_MESH_IMPORTER_HPP