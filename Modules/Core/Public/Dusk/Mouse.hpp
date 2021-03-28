#ifndef DUSK_MOUSE_HPP
#define DUSK_MOUSE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/String.hpp>

namespace Dusk {

enum class MouseButton
{
    Unknown = -1,
    
    Left,
    Right,
    Middle,
    
}; // enum MouseButton

static inline string GetMouseButtonName(MouseButton button)
{
    switch (button) {
    case MouseButton::Left:
        return "Left";
    case MouseButton::Right:
        return "Right";
    case MouseButton::Middle:
        return "Middle";
    default: ;
    }

    return string();
};

} // namespace Dusk

#endif // DUSK_MOUSE_HPP
