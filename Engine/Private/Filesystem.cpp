#include <Dusk/Filesystem.hpp>

namespace Dusk {

static IFilesystem * _Filesystem = nullptr;

DUSK_ENGINE_API
void SetFilesystem(IFilesystem * fs) {
    if (_Filesystem) {
        delete _Filesystem;
    }
    _Filesystem = fs;
}

DUSK_ENGINE_API
IFilesystem * GetFilesystem() {
    return _Filesystem;
}

} // namespace Dusk