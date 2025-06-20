#include <Dusk/Input.hpp>

namespace dusk {
namespace Input {

// Callbacks

std::function<void(SDL_Event *)> _mouseCallback;
std::function<void(SDL_Event *)> _keyboardCallback;
std::function<void(SDL_Event *)> _joystickCallback;
std::function<void(SDL_Event *)> _gamepadCallback;

DUSK_API
void SetMouseCallback(std::function<void(SDL_Event *)> mouseCallback)
{
    _mouseCallback = mouseCallback;
}

DUSK_API
void SetKeyboardCallback(std::function<void(SDL_Event *)> keyboardCallback)
{
    _keyboardCallback = keyboardCallback;
}

DUSK_API
void SetJoystickCallback(std::function<void(SDL_Event *)> joystickCallback)
{
    _joystickCallback = joystickCallback;
}

DUSK_API
void SetGamepadCallback(std::function<void(SDL_Event *)> gamepadCallback)
{
    _gamepadCallback = gamepadCallback;
}

DUSK_API
void HandleMouseEvent(SDL_Event * event)
{
    if (_mouseCallback) {
        _mouseCallback(event);
    }
}

DUSK_API
void HandleKeyboardEvent(SDL_Event * event)
{
    if (_keyboardCallback) {
        _keyboardCallback(event);
    }
}

DUSK_API
void HandleJoystickEvent(SDL_Event * event)
{
    if (_joystickCallback) {
        _joystickCallback(event);
    }
}

DUSK_API
void HandleGamepadEvent(SDL_Event * event)
{
    if (_gamepadCallback) {
        _gamepadCallback(event);
    }
}

} // namespace Input
} // namespace dusk