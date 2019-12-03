#ifndef DUSK_STBI_TEXTURE_LOADER_HPP
#define DUSK_STBI_TEXTURE_LOADER_HPP

#include <Dusk/STBI.hpp>
#include <Dusk/Loaders/TextureLoader.hpp>

namespace Dusk::STBI {

class DUSK_STBI_API TextureLoader : public ITextureLoader
{
public:

    TextureData LoadFromFile(const std::string& filename) override;

    TextureData LoadFromMemory(const std::string& filename, uint8_t * data, size_t length) override;

}; // class TextureLoader

} // class 

#endif // DUSK_STBI_TEXTURE_LOADER_HPP
