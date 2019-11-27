#ifndef DUSK_OPENGL_GRAPHICS_DRIVER_HPP
#define DUSK_OPENGL_GRAPHICS_DRIVER_HPP

#include <Dusk/Drivers/GraphicsDriver.hpp>

#include <SDL.h>

namespace Dusk::OpenGL {

class GraphicsDriver : public IGraphicsDriver
{
public:

    GraphicsDriver();

    virtual ~GraphicsDriver();

    void SetTitle(const std::string& title) override;

    std::string GetTitle() override;

    void SetSize(const ivec2& size) override;

    ivec2 GetSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

private:

    SDL_Window * _sdlWindow = nullptr;

    SDL_GLContext _sdlContext;
    
}; // class GraphicsDriver

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_GRAPHICS_DRIVER_HPP