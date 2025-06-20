#include <Dusk/Dusk.hpp>

#include <SDL3/SDL.h>

#include <thread>
#include <chrono>
using namespace std::chrono_literals;

namespace dusk {

DUSK_API
void Init()
{
    Graphics::Init();
}

DUSK_API
void Term()
{
    Graphics::Term();
}

DUSK_API
void Run()
{
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type >= SDL_EVENT_WINDOW_FIRST and event.type <= SDL_EVENT_WINDOW_LAST) {
                Graphics::HandleEvent(&event);
            }

            // TODO: Regenerate the swapchain?
            // SDL_EVENT_RENDER_TARGETS_RESET
            // SDL_EVENT_RENDER_DEVICE_RESET
            // SDL_EVENT_RENDER_DEVICE_LOST

            switch (event.type) {
            case SDL_EVENT_MOUSE_MOTION:
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
            case SDL_EVENT_MOUSE_WHEEL:
                Input::HandleMouseEvent(&event);
                break;
            
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
                Input::HandleKeyboardEvent(&event);
                break;

            case SDL_EVENT_JOYSTICK_AXIS_MOTION:
            case SDL_EVENT_JOYSTICK_BALL_MOTION:
            case SDL_EVENT_JOYSTICK_HAT_MOTION:
            case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
            case SDL_EVENT_JOYSTICK_BUTTON_UP:
            case SDL_EVENT_JOYSTICK_ADDED:
            case SDL_EVENT_JOYSTICK_REMOVED:
            case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
                Input::HandleJoystickEvent(&event);
                break;

            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            case SDL_EVENT_GAMEPAD_BUTTON_UP:
            case SDL_EVENT_GAMEPAD_ADDED:
            case SDL_EVENT_GAMEPAD_REMOVED:
            case SDL_EVENT_GAMEPAD_REMAPPED:
            case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
            case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
            case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
            case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
            case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
            case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
                Input::HandleGamepadEvent(&event);
                break;
            }
        }

        Graphics::Render();

        std::this_thread::sleep_for(16ms);
    }
}

} // namespace dusk
