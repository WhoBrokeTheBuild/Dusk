#include <Dusk/GLTF2/MeshImporter.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Util.hpp>

namespace Dusk::GLTF2 {

DUSK_GLTF2_API
std::vector<std::unique_ptr<Dusk::MeshData>> MeshImporter::LoadFromFile(const std::string& filename)
{
    DuskBenchmarkStart();

    const std::string& dir = GetDirname(filename);
    std::vector<std::unique_ptr<Dusk::MeshData>> meshes;

    DuskLogInfo("%s %s", dir, filename);


    DuskBenchmarkEnd("GLTF2::MeshImporter::LoadFromFile");
        return { };
}

} // namespace Dusk::GLTF2