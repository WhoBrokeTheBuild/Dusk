#include <Dusk/Importers/TextureImporter.hpp>

#include <unordered_map>

namespace Dusk {

static std::unordered_map<std::string, std::shared_ptr<ITextureImporter>> _TextureImporters;

DUSK_CORE_API
void SetTextureImporter(const std::string& ext, std::shared_ptr<ITextureImporter> loader) {
    _TextureImporters[ext] = loader;
}

DUSK_CORE_API
ITextureImporter * GetTextureImporter(const std::string& ext) {
    auto it = _TextureImporters.find(ext);
    if (it == _TextureImporters.end()) {
        return nullptr;
    }
    return (*it).second.get();
}

DUSK_CORE_API
void FreeTextureImporter(const std::string& ext) {
    auto it = _TextureImporters.find(ext);
    if (it != _TextureImporters.end()) {
        _TextureImporters.erase(it);
    }
}

DUSK_CORE_API
void FreeAllTextureImporters() {
    _TextureImporters.clear();
}

} // namespace Dusk
