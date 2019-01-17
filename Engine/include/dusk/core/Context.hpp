#ifndef DUSK_CONTEXT_HPP
#define DUSK_CONTEXT_HPP

#include <dusk/core/OpenGL.hpp>

namespace dusk
{

class Shader;
class Camera;

struct UpdateContext
{
    float TargetFPS = 0.0f;
    
    float CurrentFPS = 0.0f;

    float DeltaTime = 0.0f;

    double ElapsedTime = 0.0f;

    double TotalTime = 0.0f;

};

struct RenderContext
{
    int NumPasses = 2;
    
    int CurrentPass = 0;

    Camera * CurrentCamera = nullptr;

    SDL_GLContext SDLGLContext;
    
};

} // namespace dusk

#endif // DUSK_CONTEXT_HPP
