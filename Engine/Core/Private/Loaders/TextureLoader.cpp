#include <Dusk/Loaders/TextureLoader.hpp>

namespace Dusk {

static ITextureLoader * _TextureLoader = nullptr;

DUSK_CORE_API void SetTextureLoader(ITextureLoader * loader)
{
    if (_TextureLoader) {
        delete _TextureLoader;
    }
    _TextureLoader = loader;
}

DUSK_CORE_API ITextureLoader * GetTextureLoader()
{
    return _TextureLoader;
}

} // namespace Dusk
