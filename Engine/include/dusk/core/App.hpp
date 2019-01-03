#ifndef DUSK_APP_HPP
#define DUSK_APP_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/core/ScriptHost.hpp>
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

    static App * Inst() { return _Inst; }

    explicit App(int argc, char** argv);
    virtual ~App();

    /// Methods

    virtual void Reset();

    void Start();
    void Stop();

    RenderContext& GetRenderContext() { return _renderContext; }
    UpdateContext& GetUpdateContext() { return _updateContext; }

    glm::ivec2 GetWindowSize() const;
    void SetWindowSize(const glm::ivec2& size);

    std::string GetWindowTitle() const { return _windowTitle; }
    void SetWindowTitle(const std::string& title);

    std::vector<glm::ivec2> GetAvailableWindowSizes();

    Shader * AddShader(std::unique_ptr<Shader>&& sp);

    Scene * AddScene(std::unique_ptr<Scene>&& scene);

    bool SetActiveScene(Scene *);
    Scene * GetActiveScene();

    /// Events

protected:

    virtual void Update();
    virtual void Render();

    virtual void ProcessSdlEvent(SDL_Event * evt);

    SDL_Window * GetSdlWindow() { return _sdlWindow; }
    SDL_GLContext GetSdlContext() { return _sdlContext; }
    
    std::vector<std::unique_ptr<Scene>>& GetScenes() { return _scenes; }
    std::vector<std::unique_ptr<Shader>>& GetShaders() { return _shaders; }

    virtual int GetSdlWindowFlags() const { return SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED; }

private:

    static inline App * _Inst = nullptr;

    void CreateWindow();
    void DestroyWindow();

    bool _running;

    std::string _windowTitle = "Dusk";
    std::string _startScene = "";

    float _targetFps = 60.0f;

    SDL_Window * _sdlWindow = nullptr;
    SDL_GLContext _sdlContext;

    RenderContext _renderContext;
    UpdateContext _updateContext;

    Scene * _activeScene = nullptr;

    std::vector<std::unique_ptr<Scene>> _scenes;

    std::vector<std::unique_ptr<Shader>> _shaders;

}; // class App

} // namespace dusk

#endif // DUSK_APP_HPP
