#include <Dusk/STBI/TextureImporter.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Math.hpp>

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

namespace Dusk::STBI {

DUSK_STBI_API
TextureData && TextureImporter::LoadFromFile(const std::string& filename) {
    int comp;
    ivec2 size;
    TextureData data;

    data.Buffer = stbi_load(filename.c_str(), &size.x, &size.y, &comp, 0);
    data.Width = size.x;
    data.Height = size.y;
    data.FreeFunc = stbi_image_free;
    data.DataType = TextureDataType::UnsignedByte;

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
        data.DataFormat = TextureDataFormat::RGBA;
    case 4:
        break;
    default:
        DuskLogError("Unknown texture data format");
    }

    return std::move(data);
}

DUSK_STBI_API
TextureData && TextureImporter::LoadFromMemory(uint8_t * buffer, size_t length) {
    TextureData data;
    return std::move(data);
}

}
