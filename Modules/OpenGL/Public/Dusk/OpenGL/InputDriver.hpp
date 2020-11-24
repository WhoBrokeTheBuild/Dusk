#ifndef DUSK_OPENGL_INPUT_DRIVER_HPP
#define DUSK_OPENGL_INPUT_DRIVER_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/Input/InputDriver.hpp>

#include <SDL.h>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API InputDriver : public Dusk::InputDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(InputDriver)

    InputDriver();

    virtual ~InputDriver();

    void ProcessEvent(SDL_Event * event);

private:

    KeyboardKey GetSDLKey(int sdlKey);

    MouseButton GetSDLMouseButton(int button);

    ControllerButton GetSDLControllerButton(int button);
    
}; // class InputDriver

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_INPUT_DRIVER_HPP
