#ifndef DUSK_TEXTURE_HPP
#define DUSK_TEXTURE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

#include <cstdint>
#include <cstdlib>
#include <string>
#include <algorithm>

namespace Dusk {

enum class DUSK_CORE_API TextureDataFormat 
{
    R,
    RG,
    RGB,
    RGBA,

}; // enum TextureDataFormat

enum class DUSK_CORE_API TextureDataCompression {
    None,
    S3TC_DXT1, // RGB, RGBA
    S3TC_DXT3, // RGBA
    S3TC_DXT5, // RGBA

}; // enum TextureDataCompression

enum class DUSK_CORE_API TextureDataType {
    UnsignedByte,
    Byte,
    UnsignedShort,
    Short,
    UnsignedInt,
    Int,

}; // enum TextureDataType

struct DUSK_CORE_API TextureData
{
    DISALLOW_COPY_AND_ASSIGN(TextureData)

    explicit TextureData() = default;

    TextureData(TextureData && rhs) {
        std::swap(Width, rhs.Width);
        std::swap(Height, rhs.Height);
        std::swap(Buffer, rhs.Buffer);
        std::swap(DataFormat, rhs.DataFormat);
        std::swap(DataCompression, rhs.DataCompression);
        std::swap(DataType, rhs.DataType);
        std::swap(FreeFunc, rhs.FreeFunc);
    }

    ~TextureData() {
        Free();
    }
    
    // Width and Height in pixels
    size_t Width;
    size_t Height;

    // Pointer to pixel data Buffer
    uint8_t * Buffer = nullptr;

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

}; // struct TextureData

class DUSK_CORE_API ITexture 
{
public:

    DISALLOW_COPY_AND_ASSIGN(ITexture)

    explicit ITexture() = default;

    virtual ~ITexture() = default;

    virtual void Bind() = 0;

    virtual bool Load(const TextureData& data) = 0;

    virtual bool LoadFromFile(const std::string& filename);

    virtual bool LoadFromMemory(uint8_t * buffer, size_t length);

}; // class ITexture

} // namespace Dusk

#endif // DUSK_TEXTURE_HPP
