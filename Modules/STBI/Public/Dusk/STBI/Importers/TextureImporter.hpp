#ifndef DUSK_STBI_TEXTURE_IMPORTER_HPP
#define DUSK_STBI_TEXTURE_IMPORTER_HPP

#include <Dusk/STBI/Config.hpp>
#include <Dusk/Importers/TextureImporter.hpp>

namespace Dusk::STBI {

class DUSK_STBI_API TextureImporter : public ITextureImporter
{
public:

    TextureData LoadFromFile(const std::string& filename) override;

    TextureData LoadFromMemory(const std::string& filename, uint8_t * data, size_t length) override;

}; // class TextureImporter

} // class 

#endif // DUSK_STBI_TEXTURE_IMPORTER_HPP
