#ifndef DUSK_TEXTURE_IMPORTER_HPP
#define DUSK_TEXTURE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Graphics/Texture.hpp>

#include <memory>
#include <vector>
#include <string>

namespace Dusk {

class DUSK_CORE_API ITextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(ITextureImporter)

    explicit ITextureImporter() = default;

    virtual ~ITextureImporter() = default;

    virtual bool LoadFromFile(TextureData& data, const std::string& filename) = 0;

    virtual bool LoadFromMemory(TextureData& data, uint8_t * buffer, size_t length) = 0;

};

DUSK_CORE_API
void AddTextureImporter(const std::string& id, std::unique_ptr<ITextureImporter> importer);

DUSK_CORE_API
void RemoveTextureImporter(const std::string& id);

DUSK_CORE_API
const std::vector<ITextureImporter *>& GetAllTextureImporters();

}

#endif // DUSK_TEXTURE_IMPORTER_HPP
