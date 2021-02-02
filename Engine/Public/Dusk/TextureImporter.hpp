#ifndef DUSK_TEXTURE_IMPORTER_HPP
#define DUSK_TEXTURE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/TextureData.hpp>
#include <Dusk/String.hpp>

#include <memory>
#include <vector>

namespace Dusk {

class DUSK_ENGINE_API TextureImporter : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureImporter)

    TextureImporter() = default;

    virtual ~TextureImporter() = default;

    virtual std::unique_ptr<TextureData> LoadFromFile(const string& filename, bool useAssetPath = true) = 0;

    virtual std::unique_ptr<TextureData> LoadFromMemory(const uint8_t * buffer, size_t length) = 0;

};

DUSK_ENGINE_API
void AddTextureImporter(const string& id, std::unique_ptr<TextureImporter> importer);

DUSK_ENGINE_API
void RemoveTextureImporter(const string& id);

DUSK_ENGINE_API
const std::vector<TextureImporter *>& GetAllTextureImporters();

}

#endif // DUSK_TEXTURE_IMPORTER_HPP
