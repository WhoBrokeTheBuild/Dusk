#include <Dusk/GLTF2/GLTF2SceneImporter.hpp>

#include "glTF2File.hpp"

namespace Dusk::GLTF2 {

DUSK_GLTF2_API
bool GLTF2SceneImporter::LoadFromFile(Entity * root, const Path& path)
{
    glTF2File file;
    file.LoadFromFile(path);

    return false;
}

} // namespace Dusk::GLTF2