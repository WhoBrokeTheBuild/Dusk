#ifndef DUSK_STBI_TEXTURE_LOADER_HPP
#define DUSK_STBI_TEXTURE_LOADER_HPP

#include <Dusk/Loaders/TextureLoader.hpp>

namespace Dusk::STBI {

class TextureLoader : public ITextureLoader
{
public:

    TextureData Load(const std::string& filename) override;

};

}

#endif // DUSK_STBI_TEXTURE_LOADER_HPP