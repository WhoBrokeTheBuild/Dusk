#include <Dusk/Module.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Drivers/OpenGLGraphicsDriver.hpp>

#include <SDL.h>

namespace Dusk::OpenGL {

void ModuleInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        DuskLogError("Failed to initialize SDL, %s", SDL_GetError());
        return;
    }

    SetGraphicsDriver(new GraphicsDriver());
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
    SDL_Quit();
}

DUSK_MODULE() {
    "OpenGL",
    ModuleInit,
    ModuleTerm,
};

} // namespace Dusk::OpenGL