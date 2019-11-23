#ifndef DUSK_TEXTURE_LOADER_HPP
#define DUSK_TEXTURE_LOADER_HPP

#include <Dusk/Macros.hpp>

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>

namespace Dusk {

enum class TextureDataFormat {
    R,
    RG,
    RGB,
    RGBA,
};

enum class TextureDataType {
    UNSIGNED_BYTE,
    BYTE,
    UNSIGNED_SHORT,
    SHORT,
    UNSIGNED_INT,
    INT,
};

struct TextureData {
    // Width and Height in pixels
    size_t Width;
    size_t Height;

    // Pointer to pixel data Buffer
    uint8_t * Buffer;

    // Format of pixels in the data
    TextureDataFormat DataFormat;

    // Format of the data in the buffer
    TextureDataType DataType;

    // Method to call to free the data
    // Set to nullptr to ignore
    void (*Free)(void *);
};

class ITextureLoader 
{
public:

    DISALLOW_COPY_AND_ASSIGN(ITextureLoader)

    explicit ITextureLoader() = default;

    virtual ~ITextureLoader() = default;

    virtual TextureData Load(const std::string& filename) = 0;

};

void SetTextureLoader(ITextureLoader * loader);

ITextureLoader * GetTextureLoader();

}

#endif // DUSK_TEXTURE_LOADER_HPP