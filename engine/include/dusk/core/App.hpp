#ifndef DUSK_APP_HPP
#define DUSK_APP_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/core/Event.hpp>
#include <dusk/core/ScriptHost.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/scene/Scene.hpp>

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace dusk {

typedef int Key;
typedef int Button;
typedef unsigned int Flags;

class App
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(App)

    explicit App(int argc, char** argv);
    virtual ~App();

    void Serialize(nlohmann::json& data);
    void Deserialize(nlohmann::json& data);

    /// Methods

    void Start();
    void Stop();

    bool LoadConfig(const std::string& filename = "");
    bool SaveConfig(const std::string& filename = "");

    RenderContext& GetRenderContext() { return _renderContext; }
    const UpdateContext& GetUpdateContext() { return _updateContext; }

    glm::ivec2 GetWindowSize() const { return _windowSize; }
    void SetWindowSize(const glm::ivec2& size);

    std::string GetWindowTitle() const { return _windowTitle; }
    void SetWindowTitle(const std::string& title);

    std::vector<glm::ivec2> GetAvailableWindowSizes();

    Shader * AddShader(std::unique_ptr<Shader>&& sp);

    inline Scene * AddScene(std::string name, Scene * scene)
    {
        return AddScene(name, std::unique_ptr<Scene>(scene));
    }

    Scene * AddScene(std::string name, std::unique_ptr<Scene>&& scene);

    bool SetActiveScene(std::string name);
    Scene * GetActiveScene();

    /// Events

    Event<> OnStart;
    Event<> OnStop;

    Event<const UpdateContext&> OnUpdate;
    Event<RenderContext&> OnRender;

    Event<Key, Flags> OnKeyPress;
    Event<Key, Flags> OnKeyRelease;

    Event<Button, glm::vec2, Flags> OnMousePress;
    Event<Button, glm::vec2, Flags> OnMouseRelease;
    Event<glm::vec2, glm::vec2, Flags> OnMouseMove;
    Event<glm::vec2> OnMouseScroll;

    Event<glm::ivec2> OnWindowResize;

    Event<std::vector<std::string>> OnFileDrop;

    Event<std::string> OnLoadConfig;

    // Dynamic Types

    static void LuaSetup(sol::state& lua) {
        lua.new_usertype<App>("App",
            "OnStart", [](App * app, std::function<void()> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnStart.AddStatic(func));
            },
            "OnStop", [](App * app, std::function<void()> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnStop.AddStatic(func));
            },
            "OnUpdate", [](App * app, std::function<void(const UpdateContext&)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnUpdate.AddStatic(func));
            },
            "OnRender", [](App * app, std::function<void(RenderContext&)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnRender.AddStatic(func));
            },
            "OnKeyPress", [](App * app, std::function<void(Key, Flags)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnKeyPress.AddStatic(func));
            },
            "OnKeyRelease", [](App * app, std::function<void(Key, Flags)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnKeyRelease.AddStatic(func));
            },
            "OnMousePress", [](App * app, std::function<void(Button, glm::vec2, Flags)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnMousePress.AddStatic(func));
            },
            "OnMouseRelease", [](App * app, std::function<void(Button, glm::vec2, Flags)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnMouseRelease.AddStatic(func));
            },
            "OnMouseMove", [](App * app, std::function<void(glm::vec2, glm::vec2, Flags)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnMouseMove.AddStatic(func));
            },
            "OnMouseScroll", [](App * app, std::function<void(glm::vec2)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnMouseScroll.AddStatic(func));
            },
            "OnWindowResize", [](App * app, std::function<void(glm::ivec2)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnWindowResize.AddStatic(func));
            },
            "OnFileDrop", [](App * app, std::function<void(std::vector<std::string>)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnFileDrop.AddStatic(func));
            },
            "OnLoadConfig", [](App * app, std::function<void(std::string)> func, sol::this_state s) {
                ScriptHost * host = sol::state_view(s)["this_script_host"];
                host->TrackCallback(app->OnLoadConfig.AddStatic(func));
            }
        );
    }

protected:

    virtual void Reset();
    virtual void Update();
    virtual void Render();
    virtual void ProcessSdlEvent(SDL_Event * evt);

    ALCdevice * GetAlDevice() { return _alDevice; }
    ALCcontext * GetAlContext() { return _alContext; }

    SDL_Window * GetSdlWindow() { return _sdlWindow; }
    SDL_GLContext GetSdlContext() { return _sdlContext; }

    std::unordered_map<std::string, std::unique_ptr<Scene>>& GetScenes() { return _scenes; }
    std::vector<std::unique_ptr<Shader>>& GetShaders() { return _shaders; }

private:

    void CreateWindow();
    void DestroyWindow();

    bool _running;

    std::string _configFilename;

    glm::ivec2 _windowSize = { 640, 480 };
    std::string _windowTitle = "Dusk";
    std::string _startScene = "";

    float _targetFps = 60.0f;

    ALCdevice * _alDevice = nullptr;
    ALCcontext * _alContext = nullptr;

    SDL_Window * _sdlWindow = nullptr;
    SDL_GLContext _sdlContext;

    RenderContext _renderContext;
    UpdateContext _updateContext;

    Scene * _activeScene = nullptr;

    std::unordered_map<std::string, std::unique_ptr<Scene>> _scenes;

    std::vector<std::unique_ptr<Shader>> _shaders;

}; // class App

} // namespace dusk

#endif // DUSK_APP_HPP
