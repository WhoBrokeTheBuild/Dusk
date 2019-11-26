#include <Dusk/Module.hpp>

#include <SDL.h>

namespace Dusk::OpenGL {

SDL_Window * _Window = nullptr;

void ModuleInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL, %s\n", SDL_GetError());
        return;
    }

    printf("Loaded SDL2\n");

    _Window = SDL_CreateWindow("Dusk", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
         1024, 768, SDL_WINDOW_OPENGL);

    SDL_Delay(1000);
}

void ModuleTerm() {
    SDL_DestroyWindow(_Window);
    SDL_Quit();
}

DUSK_MODULE() {
    .Name = "OpenGL",
    .Init = ModuleInit,
    .Term = ModuleTerm,
};

}