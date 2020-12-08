#ifndef DUSK_STBI_TEXTURE_IMPORTER_HPP
#define DUSK_STBI_TEXTURE_IMPORTER_HPP

#include <Dusk/STBI/STBIConfig.hpp>

#include <Dusk/TextureImporter.hpp>

namespace Dusk {

class DUSK_STBI_API STBITextureData : public TextureData
{
public:

    uint8_t * Data = nullptr;

    uvec2 Size;

    int Components;

    virtual ~STBITextureData();

    inline std::string GetClassID() const override {
        return "Dusk::STBITextureData";
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

    DataType GetDataType() const override {
        return TextureData::DataType::UnsignedByte;
    }

}; // class STBITextureData

class DUSK_STBI_API STBITextureImporter : public TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(STBITextureImporter)

    STBITextureImporter() = default;

    inline std::string GetClassID() const override {
        return "Dusk::STBITextureImporter";
    }

    std::unique_ptr<TextureData> LoadFromFile(const std::string& filename) override;

    std::unique_ptr<TextureData> LoadFromMemory(const uint8_t * buffer, size_t length) override;

}; // class STBITextureImporter

} // class 

#endif // DUSK_STBI_TEXTURE_IMPORTER_HPP
