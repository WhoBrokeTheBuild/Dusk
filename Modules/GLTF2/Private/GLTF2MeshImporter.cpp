#include <Dusk/GLTF2/GLTF2MeshImporter.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/GLTF2/GLTF2PrimitiveData.hpp>

#include "glTF2File.hpp"

namespace Dusk::GLTF2 {

DUSK_GLTF2_API
std::vector<std::unique_ptr<PrimitiveData>> GLTF2MeshImporter::LoadFromFile(const string& filename, bool useAssetPath /*= true*/)
{
    DuskBenchmarkStart();

    glTF2File file;
    bool result = false;

    if (useAssetPath) {
        const auto& assetPathList = GetAssetPathList();

        for (const auto& path : assetPathList) {
            Path fullPath = path / "Models" / filename;
            DuskLogVerbose("Checking '%s'", fullPath);

            result = file.LoadFromFile(fullPath);

            if (result) {
                break;
            }
        }
    }
    else {
        result = file.LoadFromFile(filename);
    }

    if (!result) {
        return { };
    }

    auto primitiveList = file.LoadMesh();

    DuskBenchmarkEnd();
    return primitiveList;
}

} // namespace Dusk::GLTF2