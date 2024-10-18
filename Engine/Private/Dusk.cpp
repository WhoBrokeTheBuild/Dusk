#include <Dusk/Dusk.hpp>

#include <SDL3/SDL.h>

#include <thread>
#include <chrono>
using namespace std::chrono_literals;

namespace dusk {

DUSK_API
void Init()
{
    Graphics::Init();
}

DUSK_API
void Term()
{
    Graphics::Term();
}

DUSK_API
void Run()
{
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type >= SDL_EVENT_WINDOW_FIRST and event.type <= SDL_EVENT_WINDOW_LAST) {
                Graphics::HandleEvent(&event);
            }
        }

        Graphics::Render();

        std::this_thread::sleep_for(16ms);
    }
}

} // namespace dusk
