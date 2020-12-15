#ifndef DUSK_TEXTURE_HPP
#define DUSK_TEXTURE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/TextureData.hpp>

#include <cstdint>
#include <cstdlib>
#include <string>
#include <tuple>
#include <memory>

namespace Dusk {

class DUSK_ENGINE_API Texture : public Object
{
public:

    enum class WrapType
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,

    }; // enum WrapType

    enum class FilterType
    {
        Nearest,
        NearestMipmapNearest,
        NearestMipmapLinear,
        Linear,
        LinearMipmapNearest,
        LinearMipmapLinear,

    }; // enum FilterType

    struct Options
    {
        Options() { }

        WrapType WrapS = WrapType::Repeat;
        WrapType WrapT = WrapType::Repeat;

        FilterType MagFilter = FilterType::Nearest;
        FilterType MinFilter = FilterType::Nearest;

        bool GenerateMipmaps = false;

    }; // struct Options

    DISALLOW_COPY_AND_ASSIGN(Texture)

    Texture() = default;

    virtual ~Texture() = default;

    virtual bool Load(const std::unique_ptr<TextureData>& data, Options opts = Options()) = 0;

}; // class Texture

DUSK_ENGINE_API
std::shared_ptr<Texture> LoadTextureFromFile(const std::string& filename, Texture::Options opts = Texture::Options());

DUSK_ENGINE_API
std::shared_ptr<Texture> LoadTextureFromMemory(const uint8_t * buffer, size_t length, Texture::Options opts = Texture::Options());

} // namespace Dusk

#endif // DUSK_TEXTURE_HPP
