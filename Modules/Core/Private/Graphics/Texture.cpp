#include <Dusk/Graphics/Texture.hpp>
#include <Dusk/Graphics/TextureImporter.hpp>

namespace Dusk {

DUSK_CORE_API
bool ITexture::LoadFromFile(const std::string& filename)
{
    TextureData data;
    const auto& importers = GetAllTextureImporters();
    for (const auto& importer : importers) {
        if (importer->LoadFromFile(data, filename)) {
            if (Load(data)) {
                return true;
            }
            data.Free();
        }
    }
    return false;
}

DUSK_CORE_API
bool ITexture::LoadFromMemory(uint8_t * buffer, size_t length)
{
    TextureData data;
    const auto& importers = GetAllTextureImporters();
    for (const auto& importer : importers) {
        if (importer->LoadFromMemory(data, buffer, length)) {
            if (Load(data)) {
                return true;
            }
            data.Free();
        }
    }
    return false;
}

} // namespace Dusk