#include <Dusk/STBI/STBITextureImporter.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/STBI/STBITextureData.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#pragma clang diagnostic pop

#pragma GCC diagnostic pop

namespace Dusk::STBI {

DUSK_STBI_API
std::unique_ptr<TextureData> STBITextureImporter::LoadFromFile(const string& filename, bool useAssetPath)
{
    DuskBenchmarkStart();

    ivec2 size;
    int components;
    uint8_t * data;

    if (useAssetPath) {
        const auto& assetPathList = Dusk::GetAssetPathList();

        for (const auto& path : assetPathList) {
            Path fullPath = path / "Textures" / filename;
            data = stbi_load(fullPath.ToCString(), &size.x, &size.y, &components, STBI_rgb_alpha);
            if (data) {
                break;
            }
        }
    }
    else {
        data = stbi_load(filename.c_str(), &size.x, &size.y, &components, STBI_rgb_alpha);
    }

    if (!data) {
        return nullptr;
    }

    DuskLogLoad("Loaded texture from '%s'", filename);

    STBITextureData * textureData = New STBITextureData();
    textureData->Data = data;
    textureData->Size = size;

    DuskBenchmarkEnd();
    return std::unique_ptr<TextureData>(textureData);
}

DUSK_STBI_API
std::unique_ptr<TextureData> STBITextureImporter::LoadFromMemory(const uint8_t * buffer, size_t length)
{
    DuskBenchmarkStart();

    ivec2 size;
    int components;

    uint8_t * data = stbi_load_from_memory(buffer, length, &size.x, &size.y, &components, STBI_rgb_alpha);
    if (!data) {
        return nullptr;
    }

    DuskLogLoad("Loaded texture from memory");

    STBITextureData * textureData = New STBITextureData();
    textureData->Data = data;
    textureData->Size = size;

    DuskBenchmarkEnd();
    return std::unique_ptr<TextureData>(textureData);
}

} // namespace Dusk::STBI
