#ifndef DUSK_OPENGL_INPUT_DRIVER_HPP
#define DUSK_OPENGL_INPUT_DRIVER_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/InputDriver.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API OpenGLInputDriver : public InputDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLInputDriver)

    OpenGLInputDriver();

    virtual ~OpenGLInputDriver();

    inline std::string GetClassID() const override {
        return "Dusk::OpenGLInputDriver";
    }

    void ProcessEvent(SDL_Event * event);

private:

    KeyboardKey GetSDLKey(int sdlKey);

    MouseButton GetSDLMouseButton(int button);

    ControllerButton GetSDLControllerButton(int button);
    
}; // class OpenGLInputDriver

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_INPUT_DRIVER_HPP
