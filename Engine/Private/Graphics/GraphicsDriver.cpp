#include <Dusk/Graphics/GraphicsDriver.hpp>

namespace Dusk {

static std::unique_ptr<GraphicsDriver> _GraphicsDriver(nullptr);

DUSK_CORE_API 
void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver)
{
    _GraphicsDriver = std::move(driver);
}

DUSK_CORE_API 
GraphicsDriver * GetGraphicsDriver()
{
    return _GraphicsDriver.get();
}

DUSK_CORE_API
UpdateContext * GraphicsDriver::GetUpdateContext()
{
    return _updateContext.get();
}

DUSK_CORE_API
RenderContext * GraphicsDriver::GetRenderContext()
{
    return _renderContext.get();
}

} // namespace Dusk