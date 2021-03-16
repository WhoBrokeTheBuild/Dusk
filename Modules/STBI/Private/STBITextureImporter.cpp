#include <Dusk/STBI/STBITextureImporter.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/STBI/STBITextureData.hpp>

DISABLE_WARNINGS()

    #define STB_IMAGE_IMPLEMENTATION
    #include <stb/stb_image.h>

ENABLE_WARNINGS()

namespace Dusk::STBI {

DUSK_STBI_API
std::vector<string> STBITextureImporter::GetSupportedMediaTypes()
{
    return {
        "image/jpeg",
        "image/png",
        "image/bmp",
        "image/tga",
        "image/gif",
    };
}

DUSK_STBI_API
std::unique_ptr<TextureData> STBITextureImporter::LoadFromFile(const Path& path, bool useAssetPath)
{
    DuskBenchmarkStart();

    ivec2 size;
    int components;
    uint8_t * data;

    if (useAssetPath) {
        const auto& assetPathList = Dusk::GetAssetPathList();

        for (const auto& assetPath : assetPathList) {
            Path fullPath = assetPath / "Textures" / path;
            data = stbi_load(fullPath.ToCString(), &size.x, &size.y, &components, STBI_rgb_alpha);
            if (data) {
                break;
            }
        }
    }
    else {
        data = stbi_load(path.ToCString(), &size.x, &size.y, &components, STBI_rgb_alpha);
    }

    if (!data) {
        return nullptr;
    }

    LogVerbose(DUSK_ANCHOR, "Loaded texture from '{}'", path);

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

    LogVerbose(DUSK_ANCHOR, "Loaded texture from memory");

    STBITextureData * textureData = New STBITextureData();
    textureData->Data = data;
    textureData->Size = size;

    DuskBenchmarkEnd();
    return std::unique_ptr<TextureData>(textureData);
}

} // namespace Dusk::STBI
