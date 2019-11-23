#ifndef DUSK_APP_HPP
#define DUSK_APP_HPP

#include <dusk/core/Context.hpp>
#include <dusk/core/Macros.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/core/OpenAL.hpp>
#include <dusk/core/OpenGL.hpp>
#include <dusk/scene/Scene.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace dusk {

typedef int Key;
typedef int Button;
typedef unsigned int Flags;

class App
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(App)

    static App * Inst() { 
        return _Inst; 
    }

    explicit App(int argc, char** argv);

    virtual ~App();

    /// Methods

    virtual void Reset();

    void Start();
    
    void Stop();

    RenderContext& GetRenderContext() { 
        return _renderContext;
    }
    
    UpdateContext& GetUpdateContext() { 
        return _updateContext;
    }

    glm::ivec2 GetWindowSize() const;
    
    void SetWindowSize(const glm::ivec2&);

    std::string GetWindowTitle() const { 
        return _windowTitle;
    }

    void SetWindowTitle(const std::string&);

    std::vector<glm::ivec2> GetAvailableWindowSizes();

    Scene * AddScene(std::unique_ptr<Scene>&&);

    bool SetActiveScene(Scene *);

    Scene * GetActiveScene();

    /// Events

protected:

    virtual void Update();

    virtual void Render();

    virtual void HandleEvent(SDL_Event *);

    SDL_Window * GetSDLWindow() { 
        return _sdlWindow;
    }

    SDL_GLContext GetSDLContext() { 
        return _sdlContext;
    }
    
    std::vector<std::unique_ptr<Scene>>& GetScenes() { 
        return _scenes;
    }

    virtual int GetSDLWindowFlags() const { 
        return SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
    }

private:

    void CreateWindow();

    void DestroyWindow();

    static inline App * _Inst = nullptr;

    bool _running;

    std::string _windowTitle = "Dusk";

    float _targetFps = 60.0f;

    ALCdevice * _alDevice = nullptr;
    
    ALCcontext * _alContext = nullptr;

    SDL_Window * _sdlWindow = nullptr;

    SDL_GLContext _sdlContext;

    RenderContext _renderContext;

    UpdateContext _updateContext;

    Scene * _activeScene = nullptr;

    std::vector<std::unique_ptr<Scene>> _scenes;

}; // class App

} // namespace dusk

#endif // DUSK_APP_HPP
