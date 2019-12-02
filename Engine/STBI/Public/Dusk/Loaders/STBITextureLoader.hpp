#ifndef DUSK_STBI_TEXTURE_LOADER_HPP
#define DUSK_STBI_TEXTURE_LOADER_HPP

#include <Dusk/STBI.hpp>
#include <Dusk/Loaders/TextureLoader.hpp>

namespace Dusk::STBI {

class DUSK_STBI_API TextureLoader : public ITextureLoader
{
public:

    TextureData Load(const std::string& filename) override;

}; // class TextureLoader

} // class 

#endif // DUSK_STBI_TEXTURE_LOADER_HPP
