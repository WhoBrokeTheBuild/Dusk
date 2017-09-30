#ifndef DUSK_APP_HPP
#define DUSK_APP_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/core/Event.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/scene/Scene.hpp>

#include <string>
#include <memory>

namespace dusk {

typedef int Key;
typedef int Button;
typedef unsigned int Flags;

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

    Shader * AddShader(std::unique_ptr<Shader>&& sp);

    Scene * AddScene(std::unique_ptr<Scene>&& scene);

    /// Events

    Event<> EvtStart;
    Event<> EvtStop;

    Event<const UpdateContext&> EvtUpdate;
    Event<RenderContext&> EvtRender;

    Event<Key, Flags> EvtKeyPress;
    Event<Key, Flags> EvtKeyRelease;

    Event<Button, glm::vec2, Flags> EvtMousePress;
    Event<Button, glm::vec2, Flags> EvtMouseRelease;
    Event<glm::vec2, glm::vec2, Flags> EvtMouseMove;
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

    std::vector<std::unique_ptr<Scene>> _scenes;

    std::vector<std::unique_ptr<Shader>> _shaders;

}; // class App

} // namespace dusk

#endif // DUSK_APP_HPP
