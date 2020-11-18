#ifndef DUSK_TEXTURE_HPP
#define DUSK_TEXTURE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Graphics/TextureImporter.hpp>

#include <cstdint>
#include <cstdlib>
#include <string>
#include <tuple>

namespace Dusk {

class DUSK_ENGINE_API Texture 
{
public:

    enum class DUSK_ENGINE_API WrapType {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,

    }; // enum WrapType

    enum class DUSK_ENGINE_API FilterType {
        Nearest,
        NearestMipmapNearest,
        NearestMipmapLinear,
        Linear,
        LinearMipmapNearest,
        LinearMipmapLinear,

    }; // enum FilterType

    struct Options
    {
        Options(
            WrapType wrapS = WrapType::Repeat,
            WrapType wrapT = WrapType::Repeat, 
            FilterType magFilter = FilterType::Nearest, 
            FilterType minFilter = FilterType::Nearest, 
            bool generateMipmaps = false)
            : WrapS(wrapS)
            , WrapT(wrapT)
            , MagFilter(magFilter)
            , MinFilter(minFilter)
            , GenerateMipmaps(generateMipmaps)
        { }
        
        WrapType WrapS;
        WrapType WrapT;

        FilterType MagFilter;
        FilterType MinFilter;

        bool GenerateMipmaps;
    };

    DISALLOW_COPY_AND_ASSIGN(Texture)

    explicit Texture() = default;

    virtual ~Texture() = default;

    virtual void Bind() = 0;

    virtual bool Load(const TextureData * data, Options opts = Options()) = 0;

    virtual bool LoadFromFile(const std::string& filename, Options opts = Options());

    virtual bool LoadFromMemory(const uint8_t * buffer, size_t length, Options opts = Options());

}; // class Texture

} // namespace Dusk

#endif // DUSK_TEXTURE_HPP
