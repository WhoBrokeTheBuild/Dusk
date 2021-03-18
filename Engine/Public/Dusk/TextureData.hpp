#ifndef DUSK_TEXTURE_DATA_HPP
#define DUSK_TEXTURE_DATA_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/String.hpp>

#include <span>

namespace Dusk {

enum class TextureDataType
{
    UnsignedByte,
    Byte,
    UnsignedShort,
    Short,
    UnsignedInt,
    Int,

}; // enum TextureDataType

class DUSK_ENGINE_API TextureData : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureData)

    TextureData() = default;

    virtual ~TextureData() = default;

    // Pointer to pixel data Buffer
    virtual uint8_t * GetData() const = 0;

    // Width and Height in pixels
    virtual uvec2 GetSize() const = 0;

    // Format of the data in the buffer
    virtual TextureDataType GetDataType() const = 0;

}; // class TextureData

class ConstantTextureData : public TextureData
{
public:

    ConstantTextureData(uint8_t * data, uvec2 size, int components, TextureDataType dataType)
        : _data(data)
        , _size(size)
        , _dataType(dataType)
    { }

    uint8_t * GetData() const override {
        return _data;
    }

    uvec2 GetSize() const override {
        return _size;
    }

    TextureDataType GetDataType() const override {
        return _dataType;
    }

private:

    uint8_t * _data;

    uvec2 _size;

    TextureDataType _dataType;

}; // class ConstantTextureData

inline string TextureDataTypeToString(TextureDataType textureDataType)
{
    switch (textureDataType) {
        case TextureDataType::UnsignedByte:
            return "UnsignedByte";
        case TextureDataType::Byte:
            return "Byte";
        case TextureDataType::UnsignedShort:
            return "UnsignedShort";
        case TextureDataType::Short:
            return "Short";
        case TextureDataType::UnsignedInt:
            return "UnsignedInt";
        case TextureDataType::Int:
            return "Int";
    }

    return "Unknown";
}

} // namespace Dusk

#endif // DUSK_TEXTURE_DATA