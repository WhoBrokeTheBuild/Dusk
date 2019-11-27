#include <Dusk/Drivers/GraphicsDriver.hpp>

namespace Dusk {

static IGraphicsDriver * _GraphicsDriver = nullptr;

void SetGraphicsDriver(IGraphicsDriver * driver) {
    if (_GraphicsDriver) {
        delete _GraphicsDriver;
    }
    _GraphicsDriver = driver;
}

IGraphicsDriver * GetGraphicsDriver() {
    return _GraphicsDriver;
}

} // namespace Dusk