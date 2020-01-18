#include <Dusk/Filesystems/Filesystem.hpp>

namespace Dusk {

static IFilesystem * _Filesystem = nullptr;

DUSK_CORE_API
void SetFilesystem(IFilesystem * fs) {
    if (_Filesystem) {
        delete _Filesystem;
    }
    _Filesystem = fs;
}

DUSK_CORE_API
IFilesystem * GetFilesystem() {
    return _Filesystem;
}

} // namespace Dusk