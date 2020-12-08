#include <Dusk/InputDriver.hpp>

namespace Dusk {

static std::unique_ptr<InputDriver> _InputDriver(nullptr);

DUSK_ENGINE_API 
void SetInputDriver(std::unique_ptr<InputDriver> && driver)
{
    _InputDriver = std::move(driver);
}

DUSK_ENGINE_API 
InputDriver * GetInputDriver()
{
    return _InputDriver.get();
}

} // namespace Dusk
