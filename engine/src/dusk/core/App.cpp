#include "dusk/core/App.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/asset/Mesh.hpp>
#include <dusk/asset/Texture.hpp>
#include <dusk/scene/Model.hpp>
#include <dusk/scene/Camera.hpp>
#include <dusk/core/ScriptPack.hpp>
#include <fstream>
#include <memory>
#include <thread>

#include <dusk/asset/loaders/MeshLoaderObj.hpp>
#include <dusk/asset/loaders/MeshLoaderFbx.hpp>

namespace dusk {

App::App(int argc, char** argv)
{
    DuskLogInfo("Starting Application");

    Mesh::AddLoader(std::unique_ptr<IMeshLoader>(new MeshLoaderObj()));
    Mesh::AddLoader(std::unique_ptr<IMeshLoader>(new MeshLoaderFbx()));

    CreateWindow();

    BaseClass::Initializers.emplace("Actor", []() -> std::unique_ptr<BaseClass> { return std::unique_ptr<Actor>(new Actor()); });
    BaseClass::Serializers.emplace("Actor", [](BaseClass * base, nlohmann::json& data) {
        Actor * inst = dynamic_cast<Actor*>(base);
        inst->Serialize(data);
    });
    BaseClass::Deserializers.emplace("Actor", [](BaseClass * base, nlohmann::json& data) {
        Actor * inst = dynamic_cast<Actor*>(base);
        inst->Deserialize(data);
    });

    BaseClass::Initializers.emplace("Model", []() -> std::unique_ptr<BaseClass> { return std::unique_ptr<Model>(new Model()); });
    BaseClass::Serializers.emplace("Model", [](BaseClass * base, nlohmann::json& data) {
        Model * inst = dynamic_cast<Model*>(base);
        inst->Serialize(data);
    });
    BaseClass::Deserializers.emplace("Model", [](BaseClass * base, nlohmann::json& data) {
        Model * inst = dynamic_cast<Model*>(base);
        inst->Deserialize(data);
    });

    BaseClass::Initializers.emplace("Camera", []() -> std::unique_ptr<BaseClass> { return std::unique_ptr<Camera>(new Camera()); });
    BaseClass::Serializers.emplace("Camera", [](BaseClass * base, nlohmann::json& data) {

    });
    BaseClass::Deserializers.emplace("Camera", [](BaseClass * base, nlohmann::json& data) {

    });
}

App::~App()
{
    DuskLogInfo("Stopping Application");

    DestroyWindow();
}

void App::Start()
{
    using namespace std::chrono;
    typedef duration<double, std::milli> double_ms;

    SDL_ShowWindow(_sdlWindow);

    OnStart.Call();

    unsigned long frames = 0;

    double_ms frameDelay = 1000ms / _targetFps;
    double_ms fpsDelay = 250ms; // Update FPS 4 times per second

    double_ms frameElap = 0ms;
    double_ms fpsElap = 0ms;

    auto timeOffset = high_resolution_clock::now();

    _updateContext.TargetFPS = _targetFps;
    _renderContext.SDLGLContext = _sdlContext;

    SDL_Event evt;

    _running = true;
    while (_running)
    {
        auto elapsedTime = duration_cast<double_ms>(high_resolution_clock::now() - timeOffset);
        timeOffset = high_resolution_clock::now();

		while (SDL_PollEvent(&evt))
		{
            ProcessSdlEvent(&evt);
		}

        _updateContext.DeltaTime = duration_cast<double_ms>(elapsedTime / frameDelay.count()).count();
        _updateContext.ElapsedTime = elapsedTime.count();
        _updateContext.TotalTime += elapsedTime.count();

        Update();

        frameElap += elapsedTime;
        if (frameDelay <= frameElap)
        {
            frameElap = 0ms;
            ++frames;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            _renderContext.CurrentPass = 0;
            _renderContext.CurrentShader = nullptr;

            Render();

            SDL_GL_SwapWindow(_sdlWindow);
        }

        fpsElap += elapsedTime;
        if (fpsDelay <= fpsElap)
        {
            _updateContext.CurrentFPS = (frames / fpsElap.count()) * 1000.0;

            static char buffer[128];
            sprintf(buffer, "%s - %0.2f", _windowTitle.c_str(), _updateContext.CurrentFPS);
            SDL_SetWindowTitle(_sdlWindow, buffer);

            frames = 0;
            fpsElap = 0ms;
        }
    }

    OnStop.Call();

    SDL_HideWindow(_sdlWindow);
}

void App::Stop()
{
    _running = false;
}

void App::Serialize(nlohmann::json& data)
{
    data["Size"] = { _windowSize.x, _windowSize.y };
    data["Title"] = _windowTitle;
    data["StartScene"] = _startScene;
}

void App::Deserialize(nlohmann::json& data)
{
    if (data.find("Size") != data.end())
    {
        _windowSize.x = data["Size"][0];
        _windowSize.y = data["Size"][1];
        SetWindowSize(_windowSize);
    }

	if (data.find("Title") != data.end())
	{
		_windowTitle = data["Title"].get<std::string>();
        SetWindowTitle(_windowTitle);
	}

    if (data.find("Scenes") != data.end())
    {
        for (const std::string& scene : data["Scenes"])
        {
            DuskLogLoad("Loading scene '%s'", scene.c_str());
            std::unique_ptr<Scene> ptr(new Scene());
            ptr->Load(scene);
            AddScene(scene, std::move(ptr));
        }
    }

    if (data.find("StartScene") != data.end())
    {
        _startScene = data["StartScene"].get<std::string>();
        SetActiveScene(_startScene);
    }
}

bool App::LoadConfig(const std::string& filename)
{
    DuskBenchStart();

    if (filename.empty())
    {
        if (_configFilename.empty())
        {
            DuskLogError("No config filename specified.");
            return false;
        }
    }
    else
    {
        _configFilename = filename;
    }

    Reset();

    std::ifstream file(_configFilename);
    nlohmann::json data;

    DuskLogLoad("Loading config file '%s'", _configFilename.c_str());

    if (!file.is_open())
    {
        DuskLogError("Failed to open config file '%s', %s", _configFilename.c_str(), strerror(errno));
        file.close();

        OnLoadConfig.Call(_configFilename);
        return false;
    }

	file >> data;
    Deserialize(data);
    file.close();

    OnLoadConfig.Call(_configFilename);

    DuskBenchEnd("App::LoadConfig()");
    return true;
}

bool App::SaveConfig(const std::string& filename)
{
    DuskBenchStart();

    if (filename.empty())
    {
        if (_configFilename.empty())
        {
            DuskLogError("No config filename specified.");
            return false;
        }
    }
    else
    {
        _configFilename = filename;
    }

    std::ofstream file(_configFilename);
    nlohmann::json data;

    DuskLogLoad("Saving config file '%s'", _configFilename.c_str());

    if (!file.is_open())
    {
        DuskLogError("Failed to open config file '%s', %s", _configFilename.c_str(), strerror(errno));
        return false;
    }

    Serialize(data);
    file << data;

    file.close();

    DuskBenchEnd("App::SaveConfig()");
    return true;
}

void App::SetWindowSize(const glm::ivec2& size)
{
    _windowSize = size;
    glViewport(0, 0, _windowSize.x, _windowSize.y);
    SDL_SetWindowSize(_sdlWindow, _windowSize.x, _windowSize.y);
}

void App::SetWindowTitle(const std::string& title)
{
    _windowTitle = title;
    SDL_SetWindowTitle(_sdlWindow, _windowTitle.c_str());
}

std::vector<glm::ivec2> App::GetAvailableWindowSizes()
{
    std::vector<glm::ivec2> sizes;

    SDL_DisplayMode tmp;

    // Get current display mode of all displays.
    for (int disp = 0; disp < SDL_GetNumVideoDisplays(); ++disp)
    {
        for (int mode = 0; mode < SDL_GetNumDisplayModes(disp); ++mode)
        {
            if (0 != SDL_GetDisplayMode(disp, mode, &tmp))
            {
                DuskLogError("Could not get display mode for video display #%d.%d: %s", disp, mode, SDL_GetError());
                break;
            }

            glm::ivec2 size = { tmp.w, tmp.h };
            if (std::find(sizes.begin(), sizes.end(), size) == sizes.end())
            {
                sizes.push_back(size);
            }
        }
    }

    std::sort(sizes.begin(), sizes.end(), [](glm::ivec2 a, glm::ivec2 b)
    {
        return a.x < b.x || a.y < b.y;
    });

    return sizes;
}

void App::Update()
{
    OnUpdate.Call(_updateContext);
    if (_activeScene) {
        _activeScene->Update(_updateContext);
    }
}

void App::Render()
{
    OnRender.Call(_renderContext);
    if (_activeScene) {
        _activeScene->Render(_renderContext);
    }
}

void App::ProcessSdlEvent(SDL_Event * evt)
{
    switch (evt->type)
    {
    case SDL_QUIT:
        _running = false;
        break;
    case SDL_KEYDOWN:
        OnKeyPress.Call(evt->key.keysym.sym, evt->key.keysym.mod);
        break;
    case SDL_KEYUP:
        OnKeyRelease.Call(evt->key.keysym.sym, evt->key.keysym.mod);
        break;
    case SDL_MOUSEMOTION:
        OnMouseMove.Call({ evt->motion.x, evt->motion.y }, { evt->motion.xrel, evt->motion.yrel }, evt->motion.state);
        break;
    case SDL_MOUSEBUTTONDOWN:
        OnMousePress.Call(evt->button.button, { evt->button.x, evt->button.y }, evt->button.state);
        break;
    case SDL_MOUSEBUTTONUP:
        OnMouseRelease.Call(evt->button.button, { evt->button.x, evt->button.y }, evt->button.state);
        break;
    case SDL_MOUSEWHEEL:
        OnMouseScroll.Call({ evt->wheel.x, evt->wheel.y });
        break;
    case SDL_WINDOWEVENT:
        if (evt->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            _windowSize = { evt->window.data1, evt->window.data2 };
            OnWindowResize.Call(_windowSize);
            glViewport(0, 0, _windowSize.x, _windowSize.y);
        }
        break;
    }
}

void App::Reset()
{
    _scenes.clear();
    _shaders.clear();

    //OnStart.RemoveAllListeners();
    //OnStop.RemoveAllListeners();

    //OnUpdate.RemoveAllListeners();
    //OnRender.RemoveAllListeners();

    //OnKeyPress.RemoveAllListeners();
    //OnKeyRelease.RemoveAllListeners();

    //OnMousePress.RemoveAllListeners();
    //OnMouseRelease.RemoveAllListeners();
    //OnMouseMove.RemoveAllListeners();
    //OnMouseScroll.RemoveAllListeners();

    //OnWindowResize.RemoveAllListeners();

    //OnFileDrop.RemoveAllListeners();

    //OnLoadConfig.RemoveAllListeners();
}

void App::CreateWindow()
{
    DuskBenchStart();

    DuskLogInfo("zlib Version %s", zlibVersion());

    _alDevice = alcOpenDevice(NULL);
    _alContext = alcCreateContext(_alDevice, NULL);
    alcMakeContextCurrent(_alContext);

    DuskLogInfo("OpenAL Version %s", alGetString(AL_VERSION));
    DuskLogInfo("OpenAL Vendor %s", alGetString(AL_VENDOR));
    DuskLogInfo("OpenAL Renderer %s", alGetString(AL_RENDERER));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        DuskLogError("Failed to initialize SDL, %s", SDL_GetError());
        return;
    }

    const std::vector<glm::ivec2>& windowSizes = GetAvailableWindowSizes();
    _windowSize = windowSizes.back();

    int sdlGlFlags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;

#ifndef NDEBUG
    sdlGlFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, sdlGlFlags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, DUSK_MIN_OPENGL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, DUSK_MIN_OPENGL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);

    int sdlWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;

    for (int multisamples = 16; multisamples > 0; multisamples /= 2)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisamples);

        _sdlWindow = SDL_CreateWindow(_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowSize.x, _windowSize.y, sdlWindowFlags);
        if (_sdlWindow) break;
    }

    if (!_sdlWindow)
    {
        DuskLogError("Failed to create SDL window, %s", SDL_GetError());
        return;
    }

    _sdlContext = SDL_GL_CreateContext(_sdlWindow);
    if (!_sdlContext)
    {
        DuskLogError("Failed to create OpenGL context, %s", SDL_GetError());
        return;
    }

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
    {
        DuskLogError("Failed to initialize OpenGL context");
        return;
    }

    DuskLogInfo("OpenGL Version %s", glGetString(GL_VERSION));
    DuskLogInfo("GLSL Version %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    DuskLogInfo("OpenGL Vendor %s", glGetString(GL_VENDOR));
    DuskLogInfo("OpenGL Renderer %s", glGetString(GL_RENDERER));

    int samples;
    glGetIntegerv(GL_SAMPLES, &samples);
    DuskLogInfo("Anti-Aliasing %dx", samples);

#ifdef DUSK_ENABLE_BINARY_SHADERS

    GLint formats = 0;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
    DuskLogInfo("Binary Shader Formats %d", formats);

#endif // DUSK_ENABLE_BINARY_SHADERS

    Shader::InitializeVersionString();
    Shader::InitializeUniformBuffers();

    // V-Sync
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // TODO: Move
    //_shaders.emplace("_default_text", std::unique_ptr<Shader>(new Shader({
    //    { GL_VERTEX_SHADER,   "assets/shaders/default/text.vs.glsl" },
    //    { GL_FRAGMENT_SHADER, "assets/shaders/default/text.fs.glsl"}
    //})));
    //_defaultFont = std::shared_ptr<Font>(new Font("assets/fonts/default.ttf", 36));

    DuskBenchEnd("App::CreateWindow()");
}

void App::DestroyWindow()
{
    SDL_GL_DeleteContext(_sdlContext);
    SDL_DestroyWindow(_sdlWindow);
    _sdlWindow = nullptr;

    SDL_Quit();

    alcDestroyContext(_alContext);
    alcCloseDevice(_alDevice);
}

Shader * App::AddShader(std::unique_ptr<Shader>&& sp)
{
    _shaders.push_back(std::move(sp));
    return _shaders.back().get();
}

Scene * App::AddScene(std::string name, std::unique_ptr<Scene>&& scene)
{
    _scenes.emplace(name, std::move(scene));
    return _scenes[name].get();
}

bool App::SetActiveScene(std::string name)
{
    if (_scenes.find(name) == _scenes.end())
    {
        return false;
    }

    _activeScene = _scenes[name].get();
    return true;
}

Scene * App::GetActiveScene()
{
    return _activeScene;
}

void App::Script_Init(asIScriptEngine * as)
{
    as->RegisterObjectType("App", 0, asOBJ_REF | asOBJ_NOHANDLE);

    as->RegisterObjectMethod("App", "void Start()", asMETHOD(App, Start), asCALL_THISCALL);
    as->RegisterObjectMethod("App", "void Stop()", asMETHOD(App, Stop), asCALL_THISCALL);

    as->RegisterObjectMethod("App", "bool LoadConfig(string)", asMETHOD(App, LoadConfig), asCALL_THISCALL);
    as->RegisterObjectMethod("App", "bool SaveConfig(string)", asMETHOD(App, SaveConfig), asCALL_THISCALL);

    //as->RegisterObjectMethod("App", "ivec2 GetWindowSize()", asMETHOD(App, GetWindowSize), asCALL_THISCALL);
    //as->RegisterObjectMethod("App", "void SetWindowSize(ivec2)", asMETHOD(App, SetWindowSize), asCALL_THISCALL);

    as->RegisterObjectMethod("App", "string GetWindowTitle()", asMETHOD(App, GetWindowTitle), asCALL_THISCALL);
    as->RegisterObjectMethod("App", "void SetWindowTitle(string)", asMETHOD(App, SetWindowTitle), asCALL_THISCALL);

    // GetAvailableWindowSizes()
    // AddShader
    // AddScene
    // SetActiveScene
    // GetActiveScene

    as->RegisterObjectMethod("App", "void OnStart(ScriptHost&, string)", asMETHOD(App, Script_OnStart), asCALL_THISCALL);
    as->RegisterObjectMethod("App", "void OnStop(ScriptHost&, string)", asMETHOD(App, Script_OnStop), asCALL_THISCALL);

    as->RegisterObjectMethod("App", "void OnUpdate(ScriptHost&, string)", asMETHOD(App, Script_OnUpdate), asCALL_THISCALL);
    as->RegisterObjectMethod("App", "void OnRender(ScriptHost&, string)", asMETHOD(App, Script_OnRender), asCALL_THISCALL);

    as->RegisterObjectMethod("App", "void OnKeyPress(ScriptHost&, string)", asMETHOD(App, Script_OnKeyPress), asCALL_THISCALL);
    as->RegisterObjectMethod("App", "void OnKeyRelease(ScriptHost&, string)", asMETHOD(App, Script_OnKeyRelease), asCALL_THISCALL);

    as->RegisterObjectMethod("App", "void OnMousePress(ScriptHost&, string)", asMETHOD(App, Script_OnMousePress), asCALL_THISCALL);
    as->RegisterObjectMethod("App", "void OnMouseRelease(ScriptHost&, string)", asMETHOD(App, Script_OnMouseRelease), asCALL_THISCALL);
    as->RegisterObjectMethod("App", "void OnMouseMove(ScriptHost&, string)", asMETHOD(App, Script_OnMouseMove), asCALL_THISCALL);
    as->RegisterObjectMethod("App", "void OnMouseScroll(ScriptHost&, string)", asMETHOD(App, Script_OnMouseScroll), asCALL_THISCALL);

    as->RegisterObjectMethod("App", "void OnWindowResize(ScriptHost&, string)", asMETHOD(App, Script_OnWindowResize), asCALL_THISCALL);
}

void App::Script_OnStart(ScriptHost * host, std::string func)
{
    TrackCallback(OnStart.AddScript(host, func, ScriptPack_None));
}

void App::Script_OnStop(ScriptHost * host, std::string func)
{
    TrackCallback(OnStop.AddScript(host, func, ScriptPack_None));
}

void App::Script_OnUpdate(ScriptHost * host, std::string func)
{
    TrackCallback(OnUpdate.AddScript(host, func, ScriptPack_UpdateContext));
}

void App::Script_OnRender(ScriptHost * host, std::string func)
{
    TrackCallback(OnRender.AddScript(host, func, ScriptPack_RenderContext));
}

void App::Script_OnKeyPress(ScriptHost * host, std::string func)
{
    TrackCallback(OnKeyPress.AddScript(host, func, ScriptPack_Key_Flags));
}

void App::Script_OnKeyRelease(ScriptHost * host, std::string func)
{
    TrackCallback(OnKeyRelease.AddScript(host, func, ScriptPack_Key_Flags));
}

void App::Script_OnMousePress(ScriptHost * host, std::string func)
{
    TrackCallback(OnMousePress.AddScript(host, func, ScriptPack_Button_vec2_Flags));
}

void App::Script_OnMouseRelease(ScriptHost * host, std::string func)
{
    TrackCallback(OnMouseRelease.AddScript(host, func, ScriptPack_Button_vec2_Flags));
}

void App::Script_OnMouseMove(ScriptHost * host, std::string func)
{
    TrackCallback(OnMouseMove.AddScript(host, func, ScriptPack_vec2_vec2_Flags));
}

void App::Script_OnMouseScroll(ScriptHost * host, std::string func)
{
    TrackCallback(OnMouseScroll.AddScript(host, func, ScriptPack_vec2));
}

void App::Script_OnWindowResize(ScriptHost * host, std::string func)
{
    TrackCallback(OnWindowResize.AddScript(host, func, ScriptPack_ivec2));
}

void App::Script_OnFileDrop(ScriptHost * host, std::string func)
{
    // TODO
}

void App::Script_OnLoadConfig(ScriptHost * host, std::string func)
{
    TrackCallback(OnLoadConfig.AddScript(host, func, ScriptPack_string));
}

} // namespace dusk
