#include <Dusk/Texture.hpp>
#include <Dusk/TextureImporter.hpp>
#include <Dusk/Log.hpp>

namespace Dusk {

DUSK_ENGINE_API
bool Texture::LoadFromFile(const std::string& filename, Options opts /*= Options()*/)
{
    const auto& importers = GetAllTextureImporters();
    for (const auto& importer : importers) {
        auto data = importer->LoadFromFile(filename);
        if (data && Load(data.get(), opts)) {
            return true;
        }
    }

    DuskLogError("Failed to load texture '%s'", filename);
    return false;
}

DUSK_ENGINE_API
bool Texture::LoadFromMemory(const uint8_t * buffer, size_t length, Options opts /*= Options()*/)
{
    const auto& importers = GetAllTextureImporters();
    for (const auto& importer : importers) {
        auto data = importer->LoadFromMemory(buffer, length);
        if (data && Load(data.get(), opts)) {
            return true;
        }
    }

    DuskLogError("Failed to load texture from memory");
    return false;
}

} // namespace Dusk