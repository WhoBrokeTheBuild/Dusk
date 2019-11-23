#include <Dusk/Loaders/TextureLoader.hpp>

namespace Dusk {

static ITextureLoader * _TextureLoader = nullptr;

void SetTextureLoader(ITextureLoader * loader) {
    _TextureLoader = loader;
}

ITextureLoader * GetTextureLoader() {
    return _TextureLoader;
}

}