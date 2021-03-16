#ifndef DUSK_GLTF2_MESH_IMPORTER_HPP
#define DUSK_GLTF2_MESH_IMPORTER_HPP

#include <Dusk/GLTF2/GLTF2Config.hpp>
#include <Dusk/MeshImporter.hpp>

#include <vector>

namespace Dusk::GLTF2 {

class DUSK_GLTF2_API GLTF2MeshImporter : public MeshImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(GLTF2MeshImporter)

    GLTF2MeshImporter() = default;

    std::vector<string> GetSupportedMediaTypes() override;

    std::vector<std::unique_ptr<PrimitiveData>> LoadFromFile(const Path& filename, bool useAssetPath = true) override;

}; // class GLTF2MeshImporter

} // namespace Dusk::GLTF2

#endif // DUSK_GLTF2_MESH_IMPORTER_HPP