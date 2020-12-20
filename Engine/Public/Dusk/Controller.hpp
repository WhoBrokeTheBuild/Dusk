#ifndef DUSK_CONTROLLER_HPP
#define DUSK_CONTROLLER_HPP

#include <Dusk/Config.hpp>

#include <string>

namespace Dusk {

enum class ControllerButton
{
    Unknown = -1,
    
    A,
    B,
    X,
    Y,
    Back,
    Guide,
    Start,
    LeftStick,
    RightStick,
    LeftShoulder,
    RightShoulder,
    DPadUp,
    DPadDown,
    DPadLeft,
    DPadRight,

}; // enum ControllerButton

static inline std::string GetControllerButtonName(ControllerButton button)
{
    switch (button) {
    case ControllerButton::A:
        return "A";
    case ControllerButton::B:
        return "B";
    case ControllerButton::X:
        return "X";
    case ControllerButton::Y:
        return "Y";
    case ControllerButton::Back:
        return "Back";
    case ControllerButton::Guide:
        return "Guide";
    case ControllerButton::Start:
        return "Start";
    case ControllerButton::LeftStick:
        return "Left Stick";
    case ControllerButton::RightStick:
        return "Right Stick";
    case ControllerButton::LeftShoulder:
        return "Left Shoulder";
    case ControllerButton::RightShoulder:
        return "Right Shoulder";
    case ControllerButton::DPadUp:
        return "D-Pad Up";
    case ControllerButton::DPadDown:
        return "D-Pad Down";
    case ControllerButton::DPadLeft:
        return "D-Pad Left";
    case ControllerButton::DPadRight:
        return "D-Pad Right";
    default: ;
    }

    return std::string();
};

} // namespace Dusk

#endif // DUSK_CONTROLLER_HPP
