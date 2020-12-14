#ifndef DUSK_TEXTURE_DATA_HPP
#define DUSK_TEXTURE_DATA_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>

#include <string>

namespace Dusk {

class DUSK_ENGINE_API TextureData : public Object
{
public:

    enum class DUSK_ENGINE_API CompressionType {
        None,

        // S3 Texture Compression (DirectX Texture Compression)
        S3TC_DXT1, // RGB, RGBA
        S3TC_DXT3, // RGBA
        S3TC_DXT5, // RGBA

    }; // enum CompressionType

    enum class DUSK_ENGINE_API DataType {
        UnsignedByte,
        Byte,
        UnsignedShort,
        Short,
        UnsignedInt,
        Int,

    }; // enum DataType

    DISALLOW_COPY_AND_ASSIGN(TextureData)

    TextureData() = default;

    virtual ~TextureData() = default;

    // Pointer to pixel data Buffer
    virtual uint8_t * GetData() const = 0;

    // Width and Height in pixels
    virtual uvec2 GetSize() const = 0;

    // Number of components (R, RG, RGB, RGBA)
    virtual int GetComponents() const = 0;

    // Compression of the pixels in the data
    virtual inline CompressionType GetCompressionType() const {
        return CompressionType::None;
    }

    // Format of the data in the buffer
    virtual DataType GetDataType() const = 0;

}; // class TextureData

} // namespace Dusk

#endif // DUSK_TEXTURE_DATA