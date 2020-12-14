#ifndef DUSK_SDL2_GRAPHICS_DRIVER_HPP
#define DUSK_SDL2_GRAPHICS_DRIVER_HPP

#include <Dusk/SDL2/SDL2Config.hpp>

#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/SDL2/SDL2InputDriver.hpp>

namespace Dusk::SDL2 {

class SDL2GraphicsDriver : public GraphicsDriver
{
public:

    bool Initialize() override;

    void Terminate() override;

    virtual bool CreateWindow(unsigned flags);

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    SDL_Window * GetSDL2Window() const {
        return _sdlWindow;
    }

private:

    SDL_Window * _sdlWindow = nullptr;

    SDL2InputDriver * _inputDriver = nullptr;

}; // class SDL2GraphicsDriver

} // namespace Dusk::SDL2

#endif // DUSK_SDL2_GRAPHICS_DRIVER_HPP