#include <Dusk/Module.hpp>
#include <Dusk/Drivers/OpenGLGraphicsDriver.hpp>

#include <SDL.h>

namespace Dusk::OpenGL {

void ModuleInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL, %s\n", SDL_GetError());
        return;
    }

    SetGraphicsDriver(new GraphicsDriver());
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
    SDL_Quit();
}

DUSK_MODULE() {
    .Name = "OpenGL",
    .Init = ModuleInit,
    .Term = ModuleTerm,
};

} // namespace Dusk::OpenGL