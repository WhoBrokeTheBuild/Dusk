#include <Dusk/Drivers/GraphicsDriver.hpp>

namespace Dusk {

static IGraphicsDriver * _GraphicsDriver = nullptr;

DUSK_CORE_API void SetGraphicsDriver(IGraphicsDriver * driver) {
    if (_GraphicsDriver) {
        delete _GraphicsDriver;
    }
    _GraphicsDriver = driver;
}

DUSK_CORE_API IGraphicsDriver * GetGraphicsDriver() {
    return _GraphicsDriver;
}

} // namespace Dusk