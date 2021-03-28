#include <Dusk/TextureImporter.hpp>

#include <unordered_map>

namespace Dusk {

static std::unordered_map<string, std::unique_ptr<TextureImporter>> _TextureImporterByID;

static std::vector<TextureImporter *> _TextureImporterList;

static std::unordered_map<string, std::vector<TextureImporter *>> _TextureImporterListByMediaType;

void updateTextureImporterCache()
{
    _TextureImporterList.clear();
    _TextureImporterListByMediaType.clear();

    for (const auto& [id, importer] : _TextureImporterByID) {
        _TextureImporterList.push_back(importer.get());

        for (const auto& mediaType : importer->GetSupportedMediaTypes()) {
            auto it = _TextureImporterListByMediaType.find(mediaType);
            if (it == _TextureImporterListByMediaType.end()) {
                _TextureImporterListByMediaType[mediaType] = std::vector<TextureImporter *>();
            }

            _TextureImporterListByMediaType[mediaType].push_back(importer.get());
        }
    }
}

DUSK_CORE_API
void AddTextureImporter(const string& id, std::unique_ptr<TextureImporter> importer)
{
    _TextureImporterByID[id] = std::move(importer);
    updateTextureImporterCache();
}

DUSK_CORE_API
void RemoveTextureImporter(const string& id)
{
    auto it = _TextureImporterByID.find(id);
    if (it != _TextureImporterByID.end()) {
        _TextureImporterByID.erase(it);
    }
    
    updateTextureImporterCache();
}

DUSK_CORE_API
const std::vector<TextureImporter *>& GetAllTextureImporters()
{
    return _TextureImporterList;
}

DUSK_CORE_API
const std::vector<TextureImporter *>& GetTextureImporterListForMediaType(std::string mediaType)
{
    auto it = _TextureImporterListByMediaType.find(mediaType);
    if (it != _TextureImporterListByMediaType.end()) {
        return it->second;
    }

    return _TextureImporterList;
}

} // namespace Dusk
