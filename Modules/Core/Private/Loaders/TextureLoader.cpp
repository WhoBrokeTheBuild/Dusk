#include <Dusk/Loaders/TextureLoader.hpp>

#include <unordered_map>

namespace Dusk {

static std::unordered_map<std::string, std::shared_ptr<ITextureLoader>> _TextureLoaders;

DUSK_CORE_API
void SetTextureLoader(const std::string& ext, std::shared_ptr<ITextureLoader> loader) {
    _TextureLoaders[ext] = loader;
}

DUSK_CORE_API
ITextureLoader * GetTextureLoader(const std::string& ext) {
    auto it = _TextureLoaders.find(ext);
    if (it == _TextureLoaders.end()) {
        return nullptr;
    }
    return (*it).second.get();
}

DUSK_CORE_API
void FreeTextureLoaders() {
    _TextureLoaders.clear();
}

} // namespace Dusk
