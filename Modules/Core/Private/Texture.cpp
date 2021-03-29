#include <Dusk/Texture.hpp>
#include <Dusk/TextureImporter.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/MediaType.hpp>

namespace Dusk {

DUSK_CORE_API
std::shared_ptr<Texture> LoadTextureFromFile(const Path& path, bool useAssetPath /*= true*/, string mediaType /*= ""*/, Texture::Options opts /*= Texture::Options()*/)
{
    auto gfx = GraphicsDriver::GetInstance();
    assert(gfx);

    string ext = path.GetExtension();
    
    if (mediaType.empty()) {
        mediaType = GetMediaTypeFromExtension(ext);
    }

    const auto& importers = GetTextureImporterListForMediaType(mediaType);
    for (const auto& importer : importers) {
        auto textureData = importer->LoadFromFile(path, useAssetPath);
        if (textureData) {
            auto texture = gfx->CreateTexture();
            if (texture->Load(textureData.get(), opts)) {
                return texture;
            }
            
            LogWarning(DUSK_ANCHOR, "Texture::Load() failed after TextureImporter::LoadFromFile() succeeded");
        }
    }

    LogError(DUSK_ANCHOR, "Failed to load Texture from file, '{}'", path);
    return nullptr;
}

DUSK_CORE_API
std::shared_ptr<Texture> LoadTextureFromMemory(const uint8_t * buffer, size_t length, string mediaType, Texture::Options opts /*= Texture::Options()*/)
{
    auto gfx = GraphicsDriver::GetInstance();
    assert(gfx);

    const auto& importers = GetTextureImporterListForMediaType(mediaType);
    for (const auto& importer : importers) {
        auto textureData = importer->LoadFromMemory(buffer, length);
        if (textureData) {
            auto texture = gfx->CreateTexture();
            if (texture->Load(textureData.get(), opts)) {
                return texture;
            }

            LogWarning(DUSK_ANCHOR, "Texture::Load() failed after TextureImporter::LoadFromMemory() succeeded");
        }
    }

    LogError(DUSK_ANCHOR, "Failed to load Texture from memory, %zu byte(s) from %p", length, buffer);
    return nullptr;
}

} // namespace Dusk