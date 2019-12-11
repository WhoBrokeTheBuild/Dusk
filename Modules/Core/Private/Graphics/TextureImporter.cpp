#include <Dusk/Graphics/TextureImporter.hpp>
#include <Dusk/Log.hpp>

#include <unordered_map>

namespace Dusk {

static std::unordered_map<std::string, std::unique_ptr<ITextureImporter>> _TextureImporters;

static std::vector<ITextureImporter *> _TextureImporterList;

void updateTextureImporterList()
{
    _TextureImporterList.clear();
    for (const auto& it : _TextureImporters) {
        _TextureImporterList.push_back(it.second.get());
    }
}

DUSK_CORE_API
void AddTextureImporter(const std::string& id, std::unique_ptr<ITextureImporter> importer)
{
    _TextureImporters[id] = std::move(importer);
    updateTextureImporterList();
}

DUSK_CORE_API
void RemoveTextureImporter(const std::string& id)
{
    auto it = _TextureImporters.find(id);
    if (it != _TextureImporters.end()) {
        _TextureImporters.erase(it);
    }
    
    updateTextureImporterList();
}

DUSK_CORE_API
const std::vector<ITextureImporter *>& GetAllTextureImporters()
{
    return _TextureImporterList;
}

} // namespace Dusk
