#ifndef DUSK_SDL2_INPUT_DRIVER_HPP
#define DUSK_SDL2_INPUT_DRIVER_HPP

#include <Dusk/SDL2/SDL2Config.hpp>

#include <Dusk/InputDriver.hpp>

namespace Dusk::SDL2 {

class DUSK_SDL2_API SDL2InputDriver : public InputDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(SDL2InputDriver)

    SDL2InputDriver() = default;

    virtual ~SDL2InputDriver() = default;

    void ProcessEvent(SDL_Event * event);

private:

    KeyboardKey GetSDLKey(int sdlKey);

    MouseButton GetSDLMouseButton(int button);

    ControllerButton GetSDLControllerButton(int button);
    
}; // class SDL2InputDriver

} // namespace Dusk::SDL2

#endif // DUSK_SDL2_INPUT_DRIVER_HPP
