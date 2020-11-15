#ifndef DUSK_TEXTURE_IMPORTER_HPP
#define DUSK_TEXTURE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Graphics/Texture.hpp>

#include <memory>
#include <vector>
#include <string>

namespace Dusk {

class DUSK_ENGINE_API TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureImporter)

    explicit TextureImporter() = default;

    virtual ~TextureImporter() = default;

    virtual std::unique_ptr<TextureData> LoadFromFile(const std::string& filename) = 0;

    virtual std::unique_ptr<TextureData> LoadFromMemory(uint8_t * buffer, size_t length) = 0;

};

DUSK_ENGINE_API
void AddTextureImporter(const std::string& id, std::unique_ptr<TextureImporter> importer);

DUSK_ENGINE_API
void RemoveTextureImporter(const std::string& id);

DUSK_ENGINE_API
const std::vector<TextureImporter *>& GetAllTextureImporters();

}

#endif // DUSK_TEXTURE_IMPORTER_HPP
