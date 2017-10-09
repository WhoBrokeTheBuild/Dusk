#include "dusk/core/App.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/asset/Mesh.hpp>
#include <dusk/asset/Texture.hpp>
#include <dusk/scene/Model.hpp>
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

    EvtStart.Call();

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
        _updateContext.ElapsedTime = elapsedTime;
        _updateContext.TotalTime += elapsedTime;

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

    EvtStop.Call();

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

        EvtLoadConfig.Call(_configFilename);
        return false;
    }

	data << file;
    Deserialize(data);
    file.close();

    EvtLoadConfig.Call(_configFilename);

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
    EvtUpdate.Call(_updateContext);
}

void App::Render()
{
    EvtRender.Call(_renderContext);
}

void App::ProcessSdlEvent(SDL_Event * evt)
{
    switch (evt->type)
    {
    case SDL_QUIT:
        _running = false;
        break;
    case SDL_KEYDOWN:
        EvtKeyPress.Call(evt->key.keysym.sym, evt->key.keysym.mod);
        break;
    case SDL_KEYUP:
        EvtKeyRelease.Call(evt->key.keysym.sym, evt->key.keysym.mod);
        break;
    case SDL_MOUSEMOTION:
        EvtMouseMove.Call({ evt->motion.x, evt->motion.y }, { evt->motion.xrel, evt->motion.yrel }, evt->motion.state);
        break;
    case SDL_MOUSEBUTTONDOWN:
        EvtMousePress.Call(evt->button.button, { evt->button.x, evt->button.y }, evt->button.state);
        break;
    case SDL_MOUSEBUTTONUP:
        EvtMouseRelease.Call(evt->button.button, { evt->button.x, evt->button.y }, evt->button.state);
        break;
    case SDL_MOUSEWHEEL:
        EvtMouseScroll.Call({ evt->wheel.x, evt->wheel.y });
        break;
    case SDL_WINDOWEVENT:
        if (evt->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            _windowSize = { evt->window.data1, evt->window.data2 };
            EvtWindowResize.Call(_windowSize);
            glViewport(0, 0, _windowSize.x, _windowSize.y);
        }
        break;
    }
}

void App::Reset()
{
    _scenes.clear();
    _shaders.clear();

    EvtStart.RemoveAllListeners();
    EvtStop.RemoveAllListeners();

    EvtUpdate.RemoveAllListeners();
    EvtRender.RemoveAllListeners();

    EvtKeyPress.RemoveAllListeners();
    EvtKeyRelease.RemoveAllListeners();

    EvtMousePress.RemoveAllListeners();
    EvtMouseRelease.RemoveAllListeners();
    EvtMouseMove.RemoveAllListeners();
    EvtMouseScroll.RemoveAllListeners();

    EvtWindowResize.RemoveAllListeners();

    EvtFileDrop.RemoveAllListeners();

    //EvtLoadConfig.RemoveAllListeners();
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
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
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

Scene * App::AddScene(std::unique_ptr<Scene>&& scene)
{
    _scenes.push_back(std::move(scene));
    return _scenes.back().get();
}

} // namespace dusk
