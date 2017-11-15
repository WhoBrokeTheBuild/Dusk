#ifndef DUSK_CONTEXT_HPP
#define DUSK_CONTEXT_HPP

#include <dusk/Config.hpp>

#include <chrono>

namespace dusk
{

class Shader;
class Camera;

struct UpdateContext
{
    float TargetFPS;
    float CurrentFPS;

    float DeltaTime;

    std::chrono::duration<double, std::milli> ElapsedTime;
    std::chrono::duration<double, std::milli> TotalTime;
};

struct RenderContext
{
    int CurrentPass;

    Shader * CurrentShader = nullptr;

    Camera * CurrentCamera = nullptr;

    SDL_GLContext SDLGLContext;
};

} // namespace dusk

#endif // DUSK_CONTEXT_HPP
