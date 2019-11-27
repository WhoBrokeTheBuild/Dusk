#include <Dusk/Loaders/TextureLoader.hpp>

namespace Dusk {

static ITextureLoader * _TextureLoader = nullptr;

void SetTextureLoader(ITextureLoader * loader) {
    if (_TextureLoader) {
        delete _TextureLoader;
    }
    _TextureLoader = loader;
}

ITextureLoader * GetTextureLoader() {
    return _TextureLoader;
}

} // namespace Dusk