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
    _windowTitle = GetApplicationName() + " (" + GetApplicationVersion().GetString() + ")";

    ivec2 size = GetWindowSize();

    if (_sdlWindow) {
        SDL_DestroyWindow(_sdlWindow);
        _sdlWindow = nullptr;
    }

    _sdlWindow = SDL_CreateWindow(_windowTitle.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        size.x, size.y, 
        flags | SDL_WINDOW_RESIZABLE);

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
                GraphicsDriver::SetWindowSize(size);

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