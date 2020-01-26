#include <Dusk/Input/InputDriver.hpp>

namespace Dusk {

static std::unique_ptr<InputDriver> _InputDriver(nullptr);

DUSK_CORE_API 
void SetInputDriver(std::unique_ptr<InputDriver> && driver)
{
    _InputDriver = std::move(driver);
}

DUSK_CORE_API 
InputDriver * GetInputDriver()
{
    return _InputDriver.get();
}

} // namespace Dusk
