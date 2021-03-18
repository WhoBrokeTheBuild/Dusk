#include <Dusk/SDL2/SDL2GraphicsDriver.hpp>

#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Exception.hpp>

namespace Dusk::SDL2 {

DUSK_SDL2_API
bool SDL2GraphicsDriver::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw Exception("Failed to initialize SDL2, {}", SDL_GetError());
    }

    SDL_version version;
    SDL_GetVersion(&version);
    LogVerbose(DUSK_ANCHOR, "SDL Version: {}.{}.{}", (int)version.major, (int)version.minor, (int)version.patch);

    _inputDriver = New SDL2InputDriver();
    SetInputDriver(std::unique_ptr<InputDriver>(_inputDriver));

    return true;
}

DUSK_SDL2_API
void SDL2GraphicsDriver::Terminate()
{
    _inputDriver = nullptr;
    SetInputDriver(nullptr);

    if (_sdlWindow) {
        SDL_DestroyWindow(_sdlWindow);
        _sdlWindow = nullptr;
    }

    SDL_Quit();
}

DUSK_SDL2_API
bool SDL2GraphicsDriver::CreateWindow(uint32_t flags)
{
    _windowTitle = GetApplicationName() + " (" + GetApplicationVersion().ToString() + ")";

    if (_sdlWindow) {
        SDL_DestroyWindow(_sdlWindow);
        _sdlWindow = nullptr;
    }

    _sdlWindow = SDL_CreateWindow(_windowTitle.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _windowSize.x, _windowSize.y, 
        flags | SDL_WINDOW_RESIZABLE);

    if (!_sdlWindow) {
        throw Exception("SDL_CreateWindow() failed, {}", SDL_GetError());
    }

    // TODO: Replace with an actual icon
    Uint16 pixels[16 * 16] = { 0xFFFF };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(
        pixels, 16, 16, 16, 16 * 2, 0xF000, 0x0F00, 0x00F0, 0x000F); // RGBA

    SDL_SetWindowIcon(_sdlWindow, surface);
    SDL_FreeSurface(surface);
    
    return true;
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
                ivec2 size = { event.window.data1, event.window.data2 };
                _windowSize = size;

                WindowResizedEventData data;
                data.Size = size;
                WindowResizedEvent.Call(&data);

                break;
            }
            }
        }
    }
}

void SDL2GraphicsDriver::UpdateWindowTitle(const string& title)
{
    SDL_SetWindowTitle(_sdlWindow, title.c_str());
}

void SDL2GraphicsDriver::UpdateWindowSize(const ivec2& size) 
{
    SDL_SetWindowSize(_sdlWindow, size.x, size.y);

    // TODO: Investigate why this wasn't done automatically from the ProcessEvents loop
    WindowResizedEventData data;
    data.Size = size;
    WindowResizedEvent.Call(&data);
}

} // namespace Dusk::SDL2