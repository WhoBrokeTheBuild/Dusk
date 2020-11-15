#include <Dusk/Graphics/Texture.hpp>
#include <Dusk/Graphics/TextureImporter.hpp>

namespace Dusk {

DUSK_ENGINE_API
bool Texture::LoadFromFile(const std::string& filename)
{
    const auto& importers = GetAllTextureImporters();
    for (const auto& importer : importers) {
        auto data = importer->LoadFromFile(filename);
        if (data && Load(data.get())) {
            return true;
        }
    }
    return false;
}

DUSK_ENGINE_API
bool Texture::LoadFromMemory(uint8_t * buffer, size_t length)
{
    const auto& importers = GetAllTextureImporters();
    for (const auto& importer : importers) {
        auto data = importer->LoadFromMemory(buffer, length);
        if (data && Load(data.get())) {
            return true;
        }
    }
    return false;
}

} // namespace Dusk