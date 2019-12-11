#ifndef DUSK_STBI_TEXTURE_IMPORTER_HPP
#define DUSK_STBI_TEXTURE_IMPORTER_HPP

#include <Dusk/STBI/Config.hpp>
#include <Dusk/Graphics/TextureImporter.hpp>

namespace Dusk::STBI {

class DUSK_STBI_API TextureImporter : public ITextureImporter
{
public:

    bool LoadFromFile(TextureData& data, const std::string& filename) override;

    bool LoadFromMemory(TextureData& data, uint8_t * buffer, size_t length) override;

}; // class TextureImporter

} // class 

#endif // DUSK_STBI_TEXTURE_IMPORTER_HPP
