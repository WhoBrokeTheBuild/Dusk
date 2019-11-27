#include <Dusk/Loaders/STBITextureLoader.hpp>

#include <Dusk/Math.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#pragma clang diagnostic pop

#pragma GCC diagnostic pop

namespace Dusk::STBI {

TextureData TextureLoader::Load(const std::string& filename) {
    int comp;
    ivec2 size;
    TextureData data;

    data.Buffer = stbi_load(filename.c_str(), &size.x, &size.y, &comp, 0);
    data.Width = size.x;
    data.Height = size.y;
    data.FreeFunc = stbi_image_free;
    data.DataType = TextureDataType::UNSIGNED_BYTE;

    switch (comp) {
    case 1:
        data.DataFormat = TextureDataFormat::R;
        break;
    case 2:
        data.DataFormat = TextureDataFormat::RG;
        break;
    case 3:
        data.DataFormat = TextureDataFormat::RGB;
        break;
    case 4:
        data.DataFormat = TextureDataFormat::RGBA;
        break;
    default:
        fprintf(stderr, "Unknown texture data format\n");
    }

    return data;
}

}