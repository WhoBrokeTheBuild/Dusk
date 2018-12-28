#ifndef DUSK_CONTEXT_HPP
#define DUSK_CONTEXT_HPP

#include <dusk/Config.hpp>

namespace dusk
{

class Shader;
class Camera;

struct UpdateContext
{
    float TargetFPS;
    float CurrentFPS;

    float DeltaTime;

    double ElapsedTime;
    double TotalTime;

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
