#ifndef DUSK_STBI_TEXTURE_IMPORTER_HPP
#define DUSK_STBI_TEXTURE_IMPORTER_HPP

#include <Dusk/STBI/Config.hpp>
#include <Dusk/Graphics/TextureImporter.hpp>

namespace Dusk::STBI {

class DUSK_STBI_API TextureData : public Dusk::TextureData
{
public:

    uint8_t * Data = nullptr;

    uvec2 Size;

    int Components;

    virtual ~TextureData();

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

}; // class TextureData

class DUSK_STBI_API TextureImporter : public Dusk::TextureImporter
{
public:

    std::unique_ptr<Dusk::TextureData> LoadFromFile(const std::string& filename) override;

    std::unique_ptr<Dusk::TextureData> LoadFromMemory(uint8_t * buffer, size_t length) override;

}; // class TextureImporter

} // class 

#endif // DUSK_STBI_TEXTURE_IMPORTER_HPP
