#ifndef DUSK_STBI_TEXTURE_DATA_HPP
#define DUSK_STBI_TEXTURE_DATA_HPP

#include <Dusk/STBI/STBIConfig.hpp>
#include <Dusk/TextureData.hpp>

namespace Dusk::STBI {

class DUSK_STBI_API STBITextureData : public TextureData
{
public:

    uint8_t * Data = nullptr;

    uvec2 Size;

    int Components;

    virtual ~STBITextureData() {
        stbi_image_free(Data);
    }

    uint8_t * GetData() const override {
        return Data;
    }

    uvec2 GetSize() const override {
        return Size;
    }

    int GetComponents() const override {
        return Components;
    }

    TextureDataType GetDataType() const override {
        return TextureDataType::UnsignedByte;
    }

}; // class STBITextureData

} // namespace Dusk::STBI

#endif // DUSK_STBI_TEXTURE_DATA_HPP