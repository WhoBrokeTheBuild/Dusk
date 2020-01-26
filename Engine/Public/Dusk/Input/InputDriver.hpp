#ifndef DUSK_INPUT_DRIVER_HPP
#define DUSK_INPUT_DRIVER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Input/Keyboard.hpp>
#include <Dusk/Input/Mouse.hpp>
#include <Dusk/Input/Controller.hpp>

#include <memory>

namespace Dusk {

class DUSK_CORE_API InputDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(InputDriver)

    explicit InputDriver() = default;
    
    virtual ~InputDriver() = default;
    
}; // class InputDriver

DUSK_CORE_API void SetInputDriver(std::unique_ptr<InputDriver> && driver);

DUSK_CORE_API InputDriver * GetInputDriver();

} // namespace Dusk

#endif // DUSK_INPUT_DRIVER_HPP
