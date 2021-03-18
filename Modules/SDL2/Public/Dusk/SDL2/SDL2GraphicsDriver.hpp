#ifndef DUSK_SDL2_GRAPHICS_DRIVER_HPP
#define DUSK_SDL2_GRAPHICS_DRIVER_HPP

#include <Dusk/SDL2/SDL2Config.hpp>

#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/SDL2/SDL2InputDriver.hpp>

namespace Dusk::SDL2 {

class DUSK_SDL2_API SDL2GraphicsDriver : public GraphicsDriver
{
public:

    bool Initialize() override;

    void Terminate() override;

    virtual bool CreateWindow(uint32_t flags);

    void ProcessEvents() override;

    SDL_Window * GetSDL2Window() const {
        return _sdlWindow;
    }

protected:

    void UpdateWindowTitle(const string& title) override;

    void UpdateWindowSize(const ivec2& size) override;

private:

    SDL_Window * _sdlWindow = nullptr;

    SDL2InputDriver * _inputDriver = nullptr;

}; // class SDL2GraphicsDriver

} // namespace Dusk::SDL2

#endif // DUSK_SDL2_GRAPHICS_DRIVER_HPP