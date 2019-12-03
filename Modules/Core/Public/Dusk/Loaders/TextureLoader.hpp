#ifndef DUSK_TEXTURE_LOADER_HPP
#define DUSK_TEXTURE_LOADER_HPP

#include <Dusk/Core.hpp>
#include <Dusk/Macros.hpp>

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>

namespace Dusk {

enum class DUSK_CORE_API TextureDataFormat 
{
    R,
    RG,
    RGB,
    RGBA,
};

enum class DUSK_CORE_API TextureDataCompression {
    None,
    S3TC_DXT1, // RGB, RGBA
    S3TC_DXT3, // RGBA
    S3TC_DXT5, // RGBA
};

enum class DUSK_CORE_API TextureDataType {
    UnsignedByte,
    Byte,
    UnsignedShort,
    Short,
    UnsignedInt,
    Int,
};

struct DUSK_CORE_API TextureData
{
    // Width and Height in pixels
    size_t Width;
    size_t Height;

    // Pointer to pixel data Buffer
    uint8_t * Buffer;

    // Format of pixels in the data
    TextureDataFormat DataFormat;

    // Compression of the pixels in the data
    TextureDataCompression DataCompression;

    // Format of the data in the buffer
    TextureDataType DataType;

    // Method to call to free the data
    // Set to nullptr to ignore
    void (*FreeFunc)(void *) = free;

    inline void Free() {
        if (FreeFunc) {
            FreeFunc((void *)Buffer);
        }
    }
};

class DUSK_CORE_API ITextureLoader 
{
public:

    DISALLOW_COPY_AND_ASSIGN(ITextureLoader)

    explicit ITextureLoader() = default;

    virtual ~ITextureLoader() = default;

    virtual TextureData LoadFromFile(const std::string& filename) = 0;

    virtual TextureData LoadFromMemory(const std::string& filename, uint8_t * data, size_t length) = 0;

};

DUSK_CORE_API
void SetTextureLoader(const std::string& ext, std::shared_ptr<ITextureLoader> loader);

DUSK_CORE_API
ITextureLoader * GetTextureLoader(const std::string& ext);

DUSK_CORE_API
void FreeTextureLoaders();

}

#endif // DUSK_TEXTURE_LOADER_HPP
