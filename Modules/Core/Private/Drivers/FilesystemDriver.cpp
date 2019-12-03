#include <Dusk/Drivers/FilesystemDriver.hpp>

namespace Dusk {

static IFilesystemDriver * _FilesystemDriver = nullptr;

void SetFilesystemDriver(IFilesystemDriver * driver) {
    if (_FilesystemDriver) {
        delete _FilesystemDriver;
    }
    _FilesystemDriver = driver;
}

IFilesystemDriver * GetFilesystemDriver() {
    return _FilesystemDriver;
}

} // namespace Dusk