#ifndef DUSK_TEXTURE_LOADER_HPP
#define DUSK_TEXTURE_LOADER_HPP

#include <Dusk/Core.hpp>
#include <Dusk/Macros.hpp>

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>

namespace Dusk {

enum class DUSK_CORE_API TextureDataFormat {
    R,
    RG,
    RGB,
    RGBA,
};

enum class DUSK_CORE_API TextureDataType {
    UNSIGNED_BYTE,
    BYTE,
    UNSIGNED_SHORT,
    SHORT,
    UNSIGNED_INT,
    INT,
};

struct DUSK_CORE_API TextureData {
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
    void (*FreeFunc)(void *);

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

    virtual TextureData Load(const std::string& filename) = 0;

};

DUSK_CORE_API void SetTextureLoader(ITextureLoader * loader);

DUSK_CORE_API ITextureLoader * GetTextureLoader();

}

#endif // DUSK_TEXTURE_LOADER_HPP
