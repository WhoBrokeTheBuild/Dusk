#ifndef DUSK_OPENGL_GRAPHICS_DRIVER_HPP
#define DUSK_OPENGL_GRAPHICS_DRIVER_HPP

#include <Dusk/OpenGL.hpp>
#include <Dusk/Drivers/GraphicsDriver.hpp>

#include <SDL.h>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API GraphicsDriver : public IGraphicsDriver
{
public:

    GraphicsDriver();

    virtual ~GraphicsDriver();

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

private:

    SDL_Window * _sdlWindow = nullptr;

    SDL_GLContext _sdlContext;
    
}; // class GraphicsDriver

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_GRAPHICS_DRIVER_HPP
