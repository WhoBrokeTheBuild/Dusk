#ifndef DUSK_STBI_TEXTURE_IMPORTER_HPP
#define DUSK_STBI_TEXTURE_IMPORTER_HPP

#include <Dusk/STBI/STBIConfig.hpp>

#include <Dusk/TextureImporter.hpp>

namespace Dusk::STBI {

class DUSK_STBI_API STBITextureImporter : public TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(STBITextureImporter)

    STBITextureImporter() = default;

    std::vector<string> GetSupportedMediaTypes() override;

    std::unique_ptr<TextureData> LoadFromFile(const Path& filename, bool useAssetPath = true) override;

    std::unique_ptr<TextureData> LoadFromMemory(const uint8_t * buffer, size_t length) override;

}; // class STBITextureImporter

} // namespace Dusk::STBI

#endif // DUSK_STBI_TEXTURE_IMPORTER_HPP
