#ifndef DUSK_TEXTURE_HPP
#define DUSK_TEXTURE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>

#include <cstdint>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <tuple>

namespace Dusk {

class DUSK_CORE_API TextureData
{
public:

    enum class DUSK_CORE_API CompressionType {
        None,

        // S3 Texture Compression (DirectX Texture Compression)
        S3TC_DXT1, // RGB, RGBA
        S3TC_DXT3, // RGBA
        S3TC_DXT5, // RGBA

    }; // enum CompressionType

    enum class DUSK_CORE_API DataType {
        UnsignedByte,
        Byte,
        UnsignedShort,
        Short,
        UnsignedInt,
        Int,

    }; // enum DataType

    enum class DUSK_CORE_API WrapType {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,

    }; // enum WrapType

    enum class DUSK_CORE_API FilterType {
        Nearest,
        Linear,

    }; // enum FilterType

    DISALLOW_COPY_AND_ASSIGN(TextureData)

    explicit TextureData() = default;

    virtual ~TextureData() = default;

    // Pointer to pixel data Buffer
    virtual uint8_t * GetData() = 0;

    // Width and Height in pixels
    virtual uvec2 GetSize() = 0;

    // Number of components (R, RG, RGB, RGBA)
    virtual int GetComponents() = 0;

    // Compression of the pixels in the data
    virtual inline CompressionType GetCompressionType() {
        return CompressionType::None;
    }

    // Format of the data in the buffer
    virtual DataType GetDataType() = 0;

    // Wrap config for (s, t)
    virtual inline std::tuple<WrapType, WrapType> GetWrapTypes() {
        return std::make_tuple(WrapType::Repeat, WrapType::Repeat);
    }

    // Filter config for (min, mag)
    virtual inline std::tuple<FilterType, FilterType> GetFilterTypes() {
        return std::make_tuple(FilterType::Nearest, FilterType::Nearest);
    }

    // Generate Mipmaps
    virtual inline bool GenerateMipmaps() {
        return true;
    }

}; // class TextureData

class DUSK_CORE_API Texture 
{
public:

    DISALLOW_COPY_AND_ASSIGN(Texture)

    explicit Texture() = default;

    virtual ~Texture() = default;

    virtual void Bind() = 0;

    virtual bool Load(const TextureData * data) = 0;

    virtual bool LoadFromFile(const std::string& filename);

    virtual bool LoadFromMemory(uint8_t * buffer, size_t length);

}; // class Texture

} // namespace Dusk

#endif // DUSK_TEXTURE_HPP
