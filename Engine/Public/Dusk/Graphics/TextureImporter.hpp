#ifndef DUSK_TEXTURE_IMPORTER_HPP
#define DUSK_TEXTURE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>

#include <memory>
#include <vector>
#include <string>

namespace Dusk {

class DUSK_ENGINE_API TextureData
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

class DUSK_ENGINE_API TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureImporter)

    TextureImporter() = default;

    virtual ~TextureImporter() = default;

    virtual std::unique_ptr<TextureData> LoadFromFile(const std::string& filename) = 0;

    virtual std::unique_ptr<TextureData> LoadFromMemory(const uint8_t * buffer, size_t length) = 0;

};

DUSK_ENGINE_API
void AddTextureImporter(const std::string& id, std::unique_ptr<TextureImporter> importer);

DUSK_ENGINE_API
void RemoveTextureImporter(const std::string& id);

DUSK_ENGINE_API
const std::vector<TextureImporter *>& GetAllTextureImporters();

}

#endif // DUSK_TEXTURE_IMPORTER_HPP
