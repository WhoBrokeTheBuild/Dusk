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

class App : public ICallbackHost
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(App)

    static App * Inst() { return _Inst; }

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
    UpdateContext& GetUpdateContext() { return _updateContext; }

    glm::ivec2 GetWindowSize() const { return _windowSize; }
    void SetWindowSize(const glm::ivec2& size);

    std::string GetWindowTitle() const { return _windowTitle; }
    void SetWindowTitle(const std::string& title);

    std::vector<glm::ivec2> GetAvailableWindowSizes();

    Shader * AddShader(std::unique_ptr<Shader>&& sp);

    Scene * AddScene(std::unique_ptr<Scene>&& scene);
    Scene * GetScene(std::string id);

    bool SetActiveScene(Scene *);
    Scene * GetActiveScene();

    /// Events

    Event<> OnStart;
    Event<> OnStop;

    Event<UpdateContext&> OnUpdate;
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

protected:

    virtual void Reset();

    virtual void Update();
    virtual void Render();

    virtual void ProcessSdlEvent(SDL_Event * evt);

    ALCdevice * GetAlDevice() { return _alDevice; }
    ALCcontext * GetAlContext() { return _alContext; }

    SDL_Window * GetSdlWindow() { return _sdlWindow; }
    SDL_GLContext GetSdlContext() { return _sdlContext; }

    std::vector<std::unique_ptr<Scene>>& GetScenes() { return _scenes; }
    std::vector<std::unique_ptr<Shader>>& GetShaders() { return _shaders; }

private:

    static App * _Inst;

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

    std::vector<std::unique_ptr<Scene>> _scenes;

    std::vector<std::unique_ptr<Shader>> _shaders;

public:

    static void Script_Init(asIScriptEngine * as);

    void Script_OnStart(ScriptHost * host, std::string func);
    void Script_OnStop(ScriptHost * host, std::string func);

    void Script_OnUpdate(ScriptHost * host, std::string func);
    void Script_OnRender(ScriptHost * host, std::string func);

    void Script_OnKeyPress(ScriptHost * host, std::string func);
    void Script_OnKeyRelease(ScriptHost * host, std::string func);

    void Script_OnMousePress(ScriptHost * host, std::string func);
    void Script_OnMouseRelease(ScriptHost * host, std::string func);
    void Script_OnMouseMove(ScriptHost * host, std::string func);
    void Script_OnMouseScroll(ScriptHost * host, std::string func);

    void Script_OnWindowResize(ScriptHost * host, std::string func);

    void Script_OnFileDrop(ScriptHost * host, std::string func);

    void Script_OnLoadConfig(ScriptHost * host, std::string func);

}; // class App

} // namespace dusk

#endif // DUSK_APP_HPP
