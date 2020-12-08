#include <Dusk/GLTF2/GLTF2SceneImporter.hpp>

#include "glTF2File.hpp"

namespace Dusk {

bool GLTF2SceneImporter::LoadFromFile(Entity * root, const std::string& filename)
{
    glTF2File file;
    return file.LoadFromFile(filename);
}

} // namespace Dusk