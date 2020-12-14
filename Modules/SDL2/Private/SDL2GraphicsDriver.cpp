#include <Dusk/SDL2/SDL2GraphicsDriver.hpp>

#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>

namespace Dusk::SDL2 {

DUSK_SDL2_API
bool SDL2GraphicsDriver::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        DuskLogError("Failed to initialize SDL, %s", SDL_GetError());
        return false;
    }

    SDL_version version;
    SDL_GetVersion(&version);
    DuskLogVerbose("SDL Version: %d.%d.%d", (int)version.major, (int)version.minor, (int)version.patch);

    _inputDriver = New SDL2InputDriver();
    SetInputDriver(std::unique_ptr<InputDriver>(_inputDriver));

    return true;
}

DUSK_SDL2_API
void SDL2GraphicsDriver::Terminate()
{
    _inputDriver = nullptr;
    SetInputDriver(nullptr);

    SDL_DestroyWindow(_sdlWindow);
    _sdlWindow = nullptr;

    SDL_Quit();
}

DUSK_SDL2_API
bool SDL2GraphicsDriver::CreateWindow(unsigned flags)
{
    std::string title = GetApplicationName() + " (" + GetApplicationVersion().GetString() + ")";

    if (_sdlWindow) {
        SDL_DestroyWindow(_sdlWindow);
        _sdlWindow = nullptr;
    }

    _sdlWindow = SDL_CreateWindow(title.c_str(), 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480, 
        flags);

    if (!_sdlWindow) {
        DuskLogError("SDL_CreateWindow() failed, %s", SDL_GetError());
        return false;
    }

    Uint16 pixels[16 * 16] = { 0xFFFF };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2,
                                                     0x0f00, 0x00f0, 0x000f, 0xf000);
    SDL_SetWindowIcon(_sdlWindow, surface);
    SDL_FreeSurface(surface);

    return true;
}

DUSK_SDL2_API
void SDL2GraphicsDriver::SetWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(_sdlWindow, title.c_str());
}

DUSK_SDL2_API
std::string SDL2GraphicsDriver::GetWindowTitle()
{
    return SDL_GetWindowTitle(_sdlWindow);
}

DUSK_SDL2_API
void SDL2GraphicsDriver::SetWindowSize(const ivec2& size)
{
    SDL_SetWindowSize(_sdlWindow, size.x, size.y);
}

DUSK_SDL2_API
ivec2 SDL2GraphicsDriver::GetWindowSize()
{
    ivec2 size;
    SDL_GetWindowSize(_sdlWindow, &size.x, &size.y);
    return size;
}

DUSK_SDL2_API
void SDL2GraphicsDriver::ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            SetRunning(false);
        }

        _inputDriver->ProcessEvent(&event);

        if (event.type == SDL_WINDOWEVENT)
        {
            switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
            {
                WindowResizedEventData data;
                data.Size = { event.window.data1, event.window.data2 };
                WindowResizedEvent.Call(&data);

                break;
            }
            }
        }
    }
}

} // namespace Dusk::SDL2