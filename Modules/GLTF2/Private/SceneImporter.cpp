#include <Dusk/GLTF2/SceneImporter.hpp>
#include <Dusk/GLTF2/glTF2File.hpp>

namespace Dusk {

bool GLTF2SceneImporter::LoadFromFile(Entity * root, const std::string& filename)
{
    glTF2File file;
    return file.LoadFromFile(filename);
}

} // namespace Dusk