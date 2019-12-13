#include <Dusk/Graphics/GraphicsDriver.hpp>

namespace Dusk {

static std::unique_ptr<IGraphicsDriver> _GraphicsDriver(nullptr);

DUSK_CORE_API void SetGraphicsDriver(std::unique_ptr<IGraphicsDriver> && driver)
{
    _GraphicsDriver = std::move(driver);
}

DUSK_CORE_API IGraphicsDriver * GetGraphicsDriver()
{
    return _GraphicsDriver.get();
}

} // namespace Dusk