#include <Dusk/STBI/STBITextureImporter.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Benchmark.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#define STB_NO_HDR
#define STB_NO_PSD
#define STB_NO_PIC
#define STB_NO_PNM
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#pragma clang diagnostic pop

#pragma GCC diagnostic pop

namespace Dusk {

STBITextureData::~STBITextureData()
{
    stbi_image_free(Data);
}

DUSK_STBI_API
std::unique_ptr<TextureData> STBITextureImporter::LoadFromFile(const std::string& filename)
{
    DuskBenchmarkStart();

    ivec2 size;
    int components;

    uint8_t * data = stbi_load(filename.c_str(), &size.x, &size.y, &components, 0);
    if (!data) {
        return nullptr;
    }

    STBITextureData * textureData = New STBITextureData();
    textureData->Data = data;
    textureData->Size = size;
    textureData->Components = components;

    DuskLogLoad("Loaded texture '%s' with STBI module", filename);

    DuskBenchmarkEnd("STBITextureImporter::LoadFromFile");
    return std::unique_ptr<TextureData>(textureData);
}

DUSK_STBI_API
std::unique_ptr<TextureData> STBITextureImporter::LoadFromMemory(const uint8_t * buffer, size_t length)
{
    DuskBenchmarkStart();

    ivec2 size;
    int components;

    uint8_t * data = stbi_load_from_memory(buffer, length, &size.x, &size.y, &components, 0);
    if (!data) {
        return nullptr;
    }

    STBITextureData * textureData = New STBITextureData();
    textureData->Data = data;
    textureData->Size = size;
    textureData->Components = components;

    DuskLogLoad("Loaded texture from memory with STBI module");

    DuskBenchmarkEnd("STBITextureImporter::LoadFromFile");
    return std::unique_ptr<TextureData>(textureData);
}

} // namespace Dusk
