#include <Dusk/GLTF2/GLTF2MeshImporter.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Util.hpp>

namespace Dusk::GLTF2 {

DUSK_GLTF2_API
std::vector<std::unique_ptr<PrimitiveData>> GLTF2MeshImporter::LoadFromFile(const std::string& filename, bool useAssetPath /*= true*/)
{
    DuskBenchmarkStart();

    const std::string& dir = GetDirname(filename);
    std::vector<std::unique_ptr<PrimitiveData>> meshes;

    DuskLogInfo("%s %s", dir, filename);


    DuskBenchmarkEnd();
    return { };
}

} // namespace Dusk::GLTF2