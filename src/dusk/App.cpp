#include "dusk/App.hpp"

#include <dusk/Log.hpp>
#include <dusk/Benchmark.hpp>
#include <dusk/Mesh.hpp>
#include <dusk/Model.hpp>
#include <dusk/Texture.hpp>
#include <dusk/Shader.hpp>
#include <fstream>
#include <memory>
#include <thread>

namespace dusk {

void App::LuaSetup(sol::state& lua)
{
    DuskBenchStart();

    /*
    lua.new_usertype<App>("App",
        "new", sol::no_constructor,
        "Start", &App::Start,
        "Stop", &App::Stop,
        "LoadConfig", &App::LoadConfig,
        "SaveConfig", &App::SaveConfig,
        "GetWindowSize", &App::GetWindowSize,
        "SetWindowSize", &App::SetWindowSize,
        "GetWindowTitle", &App::GetWindowTitle,
        "SetWindowTitle", &App::SetWindowTitle,
        "GetAvailableWindowSizes", &App::GetAvailableWindowSizes,

        "EvtStart", &App::EvtStart,
        "EvtStop", &App::EvtStop,
        "EvtUpdate", &App::EvtUpdate,
        "EvtRender", &App::EvtRender,
        "EvtKeyPress", &App::EvtKeyPress,
        "EvtKeyRelease", &App::EvtKeyRelease,
        "EvtMousePress", &App::EvtMousePress,
        "EvtMouseRelease", &App::EvtMouseRelease,
        "EvtMouseMove", &App::EvtMouseMove,
        "EvtMouseScroll", &App::EvtMouseScroll,
        "EvtWindowResize", &App::EvtWindowResize,
        "EvtFileDrop", &App::EvtFileDrop
    );

    lua.new_usertype<UpdateContext>("UpdateContext",
        "new", sol::no_constructor,
        "TargetFPS", &UpdateContext::TargetFPS,
        "CurrentFPS", &UpdateContext::CurrentFPS,
        "DeltaTime", &UpdateContext::DeltaTime,
        "ElapsedTime", &UpdateContext::ElapsedTime,
        "TotalTime", &UpdateContext::TotalTime
    );
    lua.new_usertype<RenderContext>("RenderContext",
        "new", sol::no_constructor,
        "CurrentPass", &RenderContext::CurrentPass,
        "CurrentShader", &RenderContext::CurrentShader,
        "CurrentCamera", &RenderContext::CurrentCamera
    );

    lua.new_simple_usertype<glm::vec2>("glm::vec2",
        "x", &glm::vec2::x,
        "y", &glm::vec2::y
    );
    lua.new_simple_usertype<glm::ivec2>("glm::ivec2",
        "x", &glm::ivec2::x,
        "y", &glm::ivec2::y
    );

    Event<>::LuaSetup(lua, "Event<>");
    Event<const UpdateContext&>::LuaSetup(lua, "Event<const UpdateContext&>");
    Event<RenderContext&>::LuaSetup(lua, "Event<RenderContext&>");
    Event<Key, Flags>::LuaSetup(lua, "Event<Key, Flags>");
    Event<Button, Flags>::LuaSetup(lua, "Event<Button, Flags>");
    Event<glm::vec2, glm::vec2>::LuaSetup(lua, "Event<glm::vec2, glm::vec2>");
    Event<glm::vec2>::LuaSetup(lua, "Event<glm::vec2>");
    Event<glm::ivec2>::LuaSetup(lua, "Event<glm::ivec2>");
    Event<std::vector<std::string>>::LuaSetup(lua, "Event<std::vector<std::string>>");

    Mesh::LuaSetup(lua);
    Model::LuaSetup(lua);
    Texture::LuaSetup(lua);
    Shader::LuaSetup(lua);
    ShaderProgram::LuaSetup(lua);

    */

    DuskBenchEnd("App::LuaSetup");
}

App::App(int argc, char** argv)
{
    DuskLogInfo("Starting Application");
    //
    // _KeyFunc = [this](int key, int scancode, int action, int mods) {
    //
    //     if (GLFW_PRESS == action)
    //     {
    //         EvtKeyPress.Call(key, mods);
    //     }
    //     else if (GLFW_RELEASE == action)
    //     {
    //         EvtKeyRelease.Call(key, mods);
    //     }
    // };
    //
    // _MouseButtonFunc = [this](int button, int action, int mods) {
    //     if (GLFW_PRESS == action)
    //     {
    //         EvtMousePress.Call(button, mods);
    //     }
    //     else if (GLFW_RELEASE == action)
    //     {
    //         EvtMouseRelease.Call(button, mods);
    //     }
    // };
    //
    // _MouseMoveFunc = [this](double x, double y) {
    //     static glm::vec2 last = { x, y };
    //     glm::vec2 cur = { x, y };
    //     EvtMouseMove.Call(cur, cur - last);
    //     last = cur;
    // };
    //
    // _ScrollFunc = [this](double xoffset, double yoffset) {
    //     EvtMouseScroll.Call(glm::vec2(xoffset, yoffset));
    // };
    //
    // _DropFunc = [this](int count, const char ** filenames) {
    //     std::vector<std::string> filenameList;
    //     for (int i = 0; i < count; ++i) filenameList.push_back(std::string(filenames[i]));
    //     EvtFileDrop.Call(filenameList);
    // };
    //
    // _WindowSizeFunc = [=](int width, int height) {
    //     EvtWindowResize.Call(glm::ivec2(width, height));
    // };

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

    UpdateContext updateCtx;
    RenderContext renderCtx;

    unsigned long frames = 0;

    double_ms frameDelay = 1000ms / _targetFps;
    double_ms fpsDelay = 250ms; // Update FPS 4 times per second

    double_ms frameElap = 0ms;
    double_ms fpsElap = 0ms;

    auto timeOffset = high_resolution_clock::now();

    updateCtx.TargetFPS = _targetFps;
    renderCtx.SDLGLContext = _sdlContext;

    SDL_Event evt;

    _running = true;
    while (_running)
    {
        auto elapsedTime = duration_cast<double_ms>(high_resolution_clock::now() - timeOffset);
        timeOffset = high_resolution_clock::now();

		while (SDL_PollEvent(&evt))
		{
            ImGui_ImplSdlGL3_ProcessEvent(&evt);
			if (evt.type == SDL_QUIT)
            {
                _running = false;
            }
		}

        ImGui_ImplSdlGL3_NewFrame(_sdlWindow);

        updateCtx.DeltaTime = duration_cast<double_ms>(elapsedTime / frameDelay.count()).count();
        updateCtx.ElapsedTime = elapsedTime;
        updateCtx.TotalTime += elapsedTime;
        EvtUpdate.Call(updateCtx);

        frameElap += elapsedTime;
        if (frameDelay <= frameElap)
        {
            frameElap = 0ms;
            ++frames;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderCtx.CurrentPass = 0;
            renderCtx.CurrentShader = nullptr;

            EvtRender.Call(renderCtx);

            SDL_GL_SwapWindow(_sdlWindow);
        }

        fpsElap += elapsedTime;
        if (fpsDelay <= fpsElap)
        {
            updateCtx.CurrentFPS = (frames / fpsElap.count()) * 1000.0;

            static char buffer[128];
            sprintf(buffer, "Dusk - %0.2f", updateCtx.CurrentFPS);
            SetWindowTitle(buffer);

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
        _windowSize.x = data["Size"][0];
    }

	if (data.find("Title") != data.end())
	{
		_windowTitle = data["Title"].get<std::string>();
	}
}

bool App::LoadConfig(const std::string& filename)
{
    DuskBenchStart();

    std::ifstream file(filename);
    nlohmann::json data;

    DuskLogLoad("Loading config file '%s'", filename.c_str());

    if (!file.is_open())
    {
        DuskLogError("Failed to open config file '%s'", filename.c_str());
        return false;
    }

	data << file;
    Deserialize(data);

    file.close();

    DuskBenchEnd("App::LoadConfig()");
    return true;
}

bool App::SaveConfig(const std::string& filename)
{
    DuskBenchStart();

    std::ofstream file(filename);
    nlohmann::json data;

    DuskLogLoad("Saving config file '%s'", filename.c_str());

    if (!file.is_open())
    {
        DuskLogError("Failed to open config file '%s'", filename.c_str());
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

void App::CreateWindow()
{
    DuskBenchStart();

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

    int sdlGlFlags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;

#ifndef NDEBUG
    sdlGlFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, sdlGlFlags);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

    int sdlWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;

    const std::vector<glm::ivec2>& windowSizes = GetAvailableWindowSizes();

    _windowSize = windowSizes.back();

    _sdlWindow = SDL_CreateWindow(_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowSize.x, _windowSize.y, sdlWindowFlags);
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

    ShaderProgram::InitializeUniformBuffers();

    ImGui_ImplSdlGL3_Init(_sdlWindow);

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
    ImGui_ImplSdlGL3_Shutdown();

    SDL_GL_DeleteContext(_sdlContext);
    SDL_DestroyWindow(_sdlWindow);
    _sdlWindow = nullptr;

    SDL_Quit();

    alcDestroyContext(_alContext);
    alcCloseDevice(_alDevice);
}

} // namespace dusk
