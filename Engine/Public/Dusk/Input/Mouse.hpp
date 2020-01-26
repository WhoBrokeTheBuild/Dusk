#ifndef DUSK_MOUSE_HPP
#define DUSK_MOUSE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

#include <string>

namespace Dusk {

enum class MouseButton {
    Unknown = -1,
    
    Left,
    Right,
    Middle,
}; // enum MouseButton

static inline std::string GetMouseButtonName(MouseButton button)
{
    switch (button) {
    case MouseButton::Left:
        return "Left";
    case MouseButton::Right:
        return "Right";
    case MouseButton::Middle:
        return "Middle";
    default:
        return "";
    }
};

} // namespace Dusk

#endif // DUSK_MOUSE_HPP
