#ifndef DUSK_TEXTURE_IMPORTER_HPP
#define DUSK_TEXTURE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/TextureData.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Path.hpp>

#include <memory>
#include <vector>

namespace Dusk {

class DUSK_CORE_API TextureImporter : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureImporter)

    TextureImporter() = default;

    virtual ~TextureImporter() = default;

    virtual std::vector<string> GetSupportedMediaTypes() {
        return { };
    }

    virtual std::unique_ptr<TextureData> LoadFromFile(const Path& filename, bool useAssetPath = true) = 0;

    virtual std::unique_ptr<TextureData> LoadFromMemory(const uint8_t * buffer, size_t length) = 0;

};

DUSK_CORE_API
void AddTextureImporter(const string& id, std::unique_ptr<TextureImporter> importer);

DUSK_CORE_API
void RemoveTextureImporter(const string& id);

DUSK_CORE_API
const std::vector<TextureImporter *>& GetTextureImporterList();

DUSK_CORE_API
const std::vector<TextureImporter *>& GetTextureImporterListForMediaType(string mediaType);

}

#endif // DUSK_TEXTURE_IMPORTER_HPP
