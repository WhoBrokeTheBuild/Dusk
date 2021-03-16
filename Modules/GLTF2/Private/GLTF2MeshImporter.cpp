#include <Dusk/GLTF2/GLTF2MeshImporter.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/Exception.hpp>
#include <Dusk/GLTF2/GLTF2PrimitiveData.hpp>

#include "glTF2File.hpp"

namespace Dusk::GLTF2 {

DUSK_GLTF2_API
std::vector<string> GLTF2MeshImporter::GetSupportedMediaTypes()
{
    return {
        "model/gltf",
        "model/gltf+json",
        "model/gltf+binary",
    };
}

DUSK_GLTF2_API
std::vector<std::unique_ptr<PrimitiveData>> GLTF2MeshImporter::LoadFromFile(const Path& filename, bool useAssetPath /*= true*/)
{
    DuskBenchmarkStart();

    glTF2File file;

    try {
        if (useAssetPath) {
            const auto& assetPathList = GetAssetPathList();

            for (const auto& path : assetPathList) {
                Path fullPath = path / "Models" / filename;
                LogVerbose(DUSK_ANCHOR, "Checking '{}'", fullPath);

                try {
                    file.LoadFromFile(fullPath);
                }
                catch (const FileNotFound& e) {
                    continue;
                }
            }
        }
        else {
            file.LoadFromFile(filename);
        }
    }
    catch (Exception& e) {
        LogVerbose(DUSK_ANCHOR, "Ignoring exception: {}", e.what());
        return { };
    }

    auto primitiveList = file.LoadMesh();

    DuskBenchmarkEnd();
    return primitiveList;
}

} // namespace Dusk::GLTF2