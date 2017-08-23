#ifndef DUSK_APP_HPP
#define DUSK_APP_HPP

#include <dusk/Config.hpp>
#include <dusk/Event.hpp>
#include <dusk/Shader.hpp>
#include <dusk/Camera.hpp>

#include <string>
#include <memory>
#include <chrono>

namespace dusk {

typedef int Key;
typedef int Button;
typedef unsigned int Flags;

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

    ShaderProgram * CurrentShader = nullptr;

    Camera * CurrentCamera = nullptr;

    SDL_GLContext SDLGLContext;
};

class App
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(App);

    static void LuaSetup(sol::state& lua);

    explicit App(int argc, char** argv);
    virtual ~App();

    void Serialize(nlohmann::json& data);
    void Deserialize(nlohmann::json& data);

    /// Methods

    void Start();
    void Stop();

    bool LoadConfig(const std::string& filename);
    bool SaveConfig(const std::string& filename);

    glm::ivec2 GetWindowSize() const { return _windowSize; }
    void SetWindowSize(const glm::ivec2& size);

    std::string GetWindowTitle() const { return _windowTitle; }
    void SetWindowTitle(const std::string& title);

    std::vector<glm::ivec2> GetAvailableWindowSizes();

    /// Events

    Event<> EvtStart;
    Event<> EvtStop;

    Event<const UpdateContext&> EvtUpdate;
    Event<RenderContext&> EvtRender;

    Event<Key, Flags> EvtKeyPress;
    Event<Key, Flags> EvtKeyRelease;

    Event<Button, Flags> EvtMousePress;
    Event<Button, Flags> EvtMouseRelease;
    Event<glm::vec2, glm::vec2> EvtMouseMove;
    Event<glm::vec2> EvtMouseScroll;

    Event<glm::ivec2> EvtWindowResize;

    Event<std::vector<std::string>> EvtFileDrop;

    // Assets

private:

    void CreateWindow();
    void DestroyWindow();

    bool _running;

    glm::ivec2 _windowSize   = { 640, 480 };
    std::string _windowTitle = "Dusk";

    float _targetFps = 60.0f;

    ALCdevice * _alDevice;
    ALCcontext * _alContext;

    SDL_Window * _sdlWindow;
    SDL_GLContext _sdlContext;

}; // class App

} // namespace dusk

#endif // DUSK_APP_HPP
