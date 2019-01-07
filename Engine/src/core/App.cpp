#include "dusk/core/App.hpp"

#include <dusk/asset/Mesh.hpp>
#include <dusk/asset/Texture.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/scene/Camera.hpp>

#include <fstream>
#include <memory>
#include <thread>

namespace dusk {

App::App(int argc, char** argv)
{
    _Inst = this;

    DuskLogInfo("Starting Application");

    CreateWindow();
}

App::~App()
{
    DuskLogInfo("Stopping Application");

    DestroyWindow();
}

void App::Reset()
{
    if (_activeScene) {
        _activeScene->Stop();
        _activeScene = nullptr;
    }

    _scenes.clear();
}

void App::Start()
{
    using namespace std::chrono;
    typedef duration<double, std::milli> double_ms;
    typedef duration<float, std::milli> float_ms;

    SDL_ShowWindow(_sdlWindow);

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
            HandleEvent(&evt);
        }

        _updateContext.DeltaTime = duration_cast<float_ms>(elapsedTime / frameDelay.count()).count();
        _updateContext.ElapsedTime = elapsedTime.count();
        _updateContext.TotalTime += elapsedTime.count();

        Update();

        frameElap += elapsedTime;
        if (frameDelay <= frameElap)
        {
            frameElap = 0ms;
            ++frames;

            glClear(GL_COLOR_BUFFER_BIT);

            for (int pass = 0; pass < _renderContext.NumPasses; ++pass) {
                _renderContext.CurrentPass = pass;
                
                glClear(GL_DEPTH_BUFFER_BIT);
                Render();
            }

            SDL_GL_SwapWindow(_sdlWindow);
        }

        fpsElap += elapsedTime;
        if (fpsDelay <= fpsElap)
        {
            _updateContext.CurrentFPS = (float)(frames / fpsElap.count()) * 1000.f;

            static char buffer[128];
            sprintf(buffer, "%s - %0.2f", _windowTitle.c_str(), _updateContext.CurrentFPS);
            SDL_SetWindowTitle(_sdlWindow, buffer);

            frames = 0;
            fpsElap = 0ms;
        }
    }

    SDL_HideWindow(_sdlWindow);
}

void App::Stop()
{
    _running = false;
}

glm::ivec2 App::GetWindowSize() const
{
    glm::ivec2 size;
    SDL_GetWindowSize(_sdlWindow, &size.x, &size.y);
    return size;
}

void App::SetWindowSize(const glm::ivec2& size)
{
    SDL_SetWindowSize(_sdlWindow, size.x, size.y);
    glViewport(0, 0, size.x, size.y);
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

    return sizes;
}

void App::Update()
{
    if (_activeScene) {
        _activeScene->Update(_updateContext);
    }
}

void App::Render()
{
    if (_activeScene) {
        _activeScene->Render(_renderContext);
    }
}

void App::HandleEvent(SDL_Event * evt)
{
    switch (evt->type)
    {
    case SDL_QUIT:
        _running = false;
        break;
    case SDL_WINDOWEVENT:
        if (evt->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            glViewport(0, 0, evt->window.data1, evt->window.data2);
        }
        break;
    }

    if (_activeScene) {
        _activeScene->HandleEvent(evt);
    }
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

    const std::vector<glm::ivec2>& windowSizes = GetAvailableWindowSizes();
    glm::ivec2 size = windowSizes.front();

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

    int sdlWindowFlags = GetSDLWindowFlags();
    DuskLogVerbose("SDL Window Flags: 0x%08X", sdlWindowFlags);

    for (int multisamples = 16; multisamples > 0; multisamples /= 2)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisamples);

        _sdlWindow = SDL_CreateWindow(_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, sdlWindowFlags);
        if (_sdlWindow) break;
    }

    if (!_sdlWindow)
    {
        DuskLogError("Failed to create SDL window, %s", SDL_GetError());
        return;
    }

    Uint16 pixels[16 * 16] = { 0xFFFF };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2,
                                                     0x0f00, 0x00f0, 0x000f, 0xf000);
    SDL_SetWindowIcon(_sdlWindow, surface);
    SDL_FreeSurface(surface);

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

    // V-Sync
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

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

Scene * App::AddScene(std::unique_ptr<Scene>&& scene)
{
    Scene * tmp = scene.get();
    _scenes.push_back(std::move(scene));
    return tmp;
}

bool App::SetActiveScene(Scene * scene)
{
    if (!scene) {
        return false;
    }

    if (_activeScene) {
        _activeScene->Stop();
    }

    _activeScene = scene;
    _activeScene->Start();
    return true;
}

Scene * App::GetActiveScene()
{
    return _activeScene;
}

} // namespace dusk
