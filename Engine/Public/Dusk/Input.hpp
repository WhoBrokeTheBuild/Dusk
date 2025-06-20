#ifndef DUSK_INPUT_HPP
#define DUSK_INPUT_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Exception.hpp>

#include <Dusk/ThirdParty/SDL.hpp>

#include <functional>

namespace dusk {

namespace Input {

    DUSK_API
    void SetMouseCallback(std::function<void(SDL_Event *)> mouseCallback);

    DUSK_API
    void SetKeyboardCallback(std::function<void(SDL_Event *)> keyboardCallback);

    DUSK_API
    void SetJoystickCallback(std::function<void(SDL_Event *)> joystickCallback);

    DUSK_API
    void SetGamepadCallback(std::function<void(SDL_Event *)> gamepadCallback);

    DUSK_API
    void HandleMouseEvent(SDL_Event * event);

    DUSK_API
    void HandleKeyboardEvent(SDL_Event * event);

    DUSK_API
    void HandleJoystickEvent(SDL_Event * event);

    DUSK_API
    void HandleGamepadEvent(SDL_Event * event);

} // namespace Input

} // namespace dusk

#endif // DUSK_INPUT_HPP