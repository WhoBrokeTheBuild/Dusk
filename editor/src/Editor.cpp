#include "Editor.hpp"
#include "ActorPanel.hpp"
#include "AssetsPanel.hpp"
#include "ComponentPanel.hpp"
#include "ShadersPanel.hpp"
#include "SettingsPanel.hpp"
#include "ScenePanel.hpp"

Editor::Editor(int argc, char ** argv)
    : dusk::App(argc, argv)
{
    DuskLogInfo("Running from the Editor");

    ImGui::CreateContext();

    ImColor c1(15, 15, 10, 255),
            c2(63, 82, 94, 255),
            c3(138, 120, 142, 255),
            c4(159, 175, 178, 255),
            c5(228, 214, 213, 255);

    std::unordered_map<std::string, ImColor> colors;
    colors["Border"] = c5;
    colors["WindowBg"] = c1;
    colors["FrameBg"] = c1;
    colors["FrameBgHovered"] = c2;
    colors["FrameBgActive"] = c1;
    colors["ChildBg"] = c1;
    colors["MenuBarBg"] = c1;
    colors["Button"] = c2;
    colors["ButtonActive"] = c1;
    colors["ButtonHovered"] = c3;
    colors["TitleBg"] = c2;
    colors["TitleBgActive"] = c2;
    colors["TitleBgCollapsed"] = c2;
    colors["Header"] = c2;
    colors["HeaderHovered"] = c3;
    colors["HeaderActive"] = c1;
    colors["CheckMark"] = c3;
    colors["SliderGrab"] = c2;
    colors["SliderGrabActive"] = c3;
    colors["ScrollbarBg"] = c1;
    colors["ScrollbarGrab"] = c2;
    colors["ScrollbarGrabHovered"] = c3;
    colors["ScrollbarGrabActive"] = c1;
    colors["PlotLines"] = c4;
    colors["PlotLinesHovered"] = c3;
    colors["PlotHistogram"] = c4;
    colors["PlotHistogramHovered"] = c3;

    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
        std::string name = ImGui::GetStyleColorName(i);
        if (colors.find(name) != colors.end()) {
            memcpy(&ImGui::GetStyle().Colors[i], &colors[name].Value, sizeof(ImVec4));
        }
    }

    ImGui::GetStyle().FrameBorderSize = 1.0f;

    _panels.emplace("Settings", std::make_unique<SettingsPanel>(this));
    _panelsLeft.push_back(_panels["Settings"].get());

    _panels.emplace("Shaders", std::make_unique<ShadersPanel>(this));
    _panelsLeft.push_back(_panels["Shaders"].get());

    _panels.emplace("Scene", std::make_unique<ScenePanel>(this));
    _panelsLeft.push_back(_panels["Scene"].get());

    _panels.emplace("Actor", std::make_unique<ActorPanel>(this));
    _panelsRight.push_back(_panels["Actor"].get());

    _panels.emplace("Component", std::make_unique<ComponentPanel>(this));
    _panelsRight.push_back(_panels["Component"].get());

    _panels.emplace("Assets", std::make_unique<AssetsPanel>(this));
    _panelsBottom.push_back(_panels["Assets"].get());

    ImBind::Init(GetSdlWindow());

    TrackCallback(OnWindowFocus.AddStatic([this](bool focused) {
        if (focused) {
            GetUpdateContext().TargetFPS = 60.0f;
        }
        else {
            GetUpdateContext().TargetFPS = 8.0f;
        }
    }));

    TrackCallback(OnMousePress.AddStatic([this](dusk::Button button, glm::vec2 pos, dusk::Flags flags) {
        if (button == SDL_BUTTON_LEFT && !_playing) {
            _cameraDrag = true;
        }
    }));

    TrackCallback(OnMouseRelease.AddStatic([this](dusk::Button button, glm::vec2 pos, dusk::Flags flags) {
        if (button == SDL_BUTTON_LEFT) {
            _cameraDrag = false;
        }
    }));

    TrackCallback(OnMouseMove.AddStatic([this](glm::vec2 pos, glm::vec2 delta, dusk::Flags flags) {
        if (_cameraDrag && _editorCamera) {
            delta *= 0.001f;
            _editorCamera->ChangePitch(delta.y);
            _editorCamera->ChangeYaw(delta.x);
        }
    }));

    TrackCallback(OnKeyPress.AddStatic([this](dusk::Key key, dusk::Flags flags) {
        if (key == SDLK_F5) {
            if (flags & KMOD_CTRL) {
                Compile();
            }
            else {
                TogglePlay();
            }
        }

        if (key == SDLK_F11) {
            _fullscreen = !_fullscreen;
        }

        if (key == SDLK_w) {
            _cameraDir.z = -1.0f;
        }
        else if (key == SDLK_s) {
            _cameraDir.z = 1.0f;
        }

        if (key == SDLK_a) {
            _cameraDir.x = -1.0f;
        }
        else if (key == SDLK_d) {
            _cameraDir.x = 1.0f;
        }

        if (key == SDLK_PAGEUP) {
            _cameraDir.y = 1.0f;
        }
        else if (key == SDLK_PAGEDOWN) {
            _cameraDir.y = -1.0f;
        }
    }));

    TrackCallback(OnKeyRelease.AddStatic([this](dusk::Key key, dusk::Flags flags) {
        if (key == SDLK_w) {
            _cameraDir.z = (_cameraDir.z < 0.0f ? 0.0f : _cameraDir.z);
        }
        else if (key == SDLK_s) {
            _cameraDir.z = (_cameraDir.z > 0.0f ? 0.0f : _cameraDir.z);
        }

        if (key == SDLK_a) {
            _cameraDir.x = (_cameraDir.x < 0.0f ? 0.0f : _cameraDir.x);
        }
        else if (key == SDLK_d) {
            _cameraDir.x = (_cameraDir.x > 0.0f ? 0.0f : _cameraDir.x);
        }

        if (key == SDLK_PAGEUP) {
            _cameraDir.y = (_cameraDir.y > 0.0f ? 0.0f : _cameraDir.y);
        }
        else if (key == SDLK_PAGEDOWN) {
            _cameraDir.y = (_cameraDir.y < 0.0f ? 0.0f : _cameraDir.y);
        }
    }));

    TrackCallback(OnWindowResize.AddStatic([this](glm::ivec2 size) {
        InitFramebuffer();
    }));

    TrackCallback(OnPlayPause.AddStatic([this](bool playing) {
        if (playing) {
            GetRenderContext().CurrentCamera = _playCamera;
            GetRenderContext().CurrentShader = _playShader;
        }
        else {
            _cameraDrag = false;

            _playCamera = GetRenderContext().CurrentCamera;
            _playShader = GetRenderContext().CurrentShader;
            GetRenderContext().CurrentCamera = _editorCamera;
            GetRenderContext().CurrentShader = _editorShader;
        }
    }));

    InitFramebuffer();

    _frameTimes.resize(20);

    SDL_SetWindowResizable(GetSdlWindow(), SDL_TRUE);
}

Editor::~Editor()
{
    TermFramebuffer();

    ImBind::Term();
    ImGui::DestroyContext();
}

void Editor::InitFramebuffer()
{
    TermFramebuffer();

    auto size = GetWindowSize();

    glGenFramebuffersEXT(1, &_glFrameBuf);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _glFrameBuf);

    glGenTextures(1, &_glTexBuf);
    glBindTexture(GL_TEXTURE_2D, _glTexBuf);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _glTexBuf, 0);

    glGenRenderbuffersEXT(1, &_glDepthBuf);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _glDepthBuf);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, size.x, size.y);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, _glDepthBuf);
}

void Editor::TermFramebuffer()
{
    glDeleteTextures(1, &_glTexBuf);
    glDeleteRenderbuffersEXT(1, &_glDepthBuf);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glDeleteFramebuffersEXT(1, &_glFrameBuf);
}

void Editor::Reset()
{
    dusk::App::Reset();

    _editorShader = nullptr;
    _editorCamera = nullptr;
    _playCamera = nullptr;
    _playShader = nullptr;

    _playing = true;
}

void Editor::Update()
{
    if (IsPlaying()) {
        dusk::App::Update();
    }
    else {
        if (_editorCamera) {
            if (glm::length2(_cameraDir) > 0.0f) {
                glm::vec3 pos = _editorCamera->GetPosition();
                glm::vec3 delta = _cameraDir * (_cameraSpeed * GetUpdateContext().DeltaTime);
                glm::vec4 tmp = glm::vec4(delta.x, delta.y, delta.z, 1.0f) * _editorCamera->GetView();
                _editorCamera->SetPosition(pos + glm::vec3(tmp.x, tmp.y, tmp.z));
            }
        }
    }
}

void Editor::Render()
{
    auto& ctx = GetUpdateContext();

    _frameTimeUpdate -= ctx.ElapsedTime;
    if (_frameTimeUpdate <= 0.0) {
        _frameTimeUpdate = 0.1;

        std::rotate(_frameTimes.begin(), _frameTimes.begin() + 1, _frameTimes.end());
        _frameTimes.back() = ctx.CurrentFPS;
    }

    ImBind::NewFrame();

    if (!GetActiveScene()) {
        SetActiveScene(AddScene(std::make_unique<dusk::Scene>("default")));
    }

    if (!_editorShader) {
        _editorShader = AddShader(std::make_unique<dusk::Shader>("_editor_shader", std::vector<std::string>({
            "shaders/default/textured.fs.glsl",
            "shaders/default/textured.vs.glsl"
        })));
    }

    if (!_editorCamera) {
        _editorCamera = GetActiveScene()->AddCamera(std::make_unique<dusk::Camera>("_editor_camera", GetActiveScene()));
        _editorCamera->SetAspect(GetWindowSize());
        _editorCamera->SetPosition({ 5, 5, 5 });
        _editorCamera->SetLookAt({ 0, 0, 0 });
    }

    if (_fullscreen) {
        dusk::App::Render();
    }
    else {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _glFrameBuf);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        dusk::App::Render();
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        glm::ivec2 winSize = GetWindowSize();
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New"))
                {

                }

                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                    LoadConfig();
                }

                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                    SaveConfig();
                }

                if (ImGui::MenuItem("Save As.."))
                {

                }

                if (ImGui::MenuItem("Quit"))
                {
                    Stop();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Editor"))
            {
                if (ImGui::MenuItem("Fullscreen", "F11", &_fullscreen))
                {
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Scene"))
            {
                if (ImGui::MenuItem("Open"))
                {
                    GetActiveScene()->Load();
                }

                if (ImGui::MenuItem("Save"))
                {
                    GetActiveScene()->Save();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Program"))
            {
                if (IsPlaying() && ImGui::MenuItem("Pause", "F5"))
                {
                    Pause();
                }
                else if (!IsPlaying() && ImGui::MenuItem("Play", "F5"))
                {
                    Play();
                }

                if (ImGui::MenuItem("Compile", "Ctrl+F5"))
                {
                    Compile();
                }

                ImGui::EndMenu();
            }

            ImGui::SameLine((float)winSize.x * 0.5f, 0.0f);
            if (IsPlaying() && ImGui::MenuItem("Pause"))
            {
                Pause();
            }
            else if (!IsPlaying() && ImGui::MenuItem("Play"))
            {
                Play();
            }

            ImGui::SameLine((float)winSize.x - 260, 0.0f);
            ImGui::PlotLines("", _frameTimes.data(), _frameTimes.size(), 0, "", 0.0f, ctx.TargetFPS, ImVec2(100, 18));

            ImGui::SameLine((float)winSize.x - 150, 0.0f);
            ImGui::Text("%.2f FPS (%.2f ms)", ctx.CurrentFPS, 1000.0f / ctx.CurrentFPS);
            ImGui::EndMainMenuBar();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0.0f);

        auto size = GetWindowSize();
        const float menuOffset = 18;
        const float leftPerc = 0.2f;
        const float centerPerc = 0.6f;
        const float rightPerc = 0.2f;

        ImGui::SetNextWindowSize(ImVec2(size.x * leftPerc, size.y - menuOffset));
        ImGui::SetNextWindowPos(ImVec2(0, menuOffset));
        if (ImGui::Begin("panel_left", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

            if (!_panelsLeft.empty()) {
                ImGui::BeginTabBar("##tabs_left");

                for (auto& p : _panelsLeft) {
                    p->Render();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGui::SetNextWindowSize(ImVec2(size.x * centerPerc, size.y * centerPerc));
        ImGui::SetNextWindowPos(ImVec2(size.x * leftPerc, menuOffset));
        if (ImGui::Begin("panel_center", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

            ImGui::Image((void *)_glTexBuf, ImVec2(size.x * centerPerc, size.y * centerPerc), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
        }

        ImGui::PopStyleVar();

        ImGui::SetNextWindowSize(ImVec2(size.x * centerPerc, size.y * (1.0f - centerPerc) - menuOffset));
        ImGui::SetNextWindowPos(ImVec2(size.x * leftPerc, size.y * centerPerc + menuOffset));
        if (ImGui::Begin("panel_bottom", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

            if (!_panelsBottom.empty()) {
                ImGui::BeginTabBar("##tabs_bottom");

                for (auto& p : _panelsBottom) {
                    p->Render();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }

        ImGui::SetNextWindowSize(ImVec2(size.x * (rightPerc), size.y - menuOffset));
        ImGui::SetNextWindowPos(ImVec2(size.x * (leftPerc + centerPerc), menuOffset));
        if (ImGui::Begin("panel_right", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

            if (!_panelsRight.empty()) {
                ImGui::BeginTabBar("##tabs_right");

                for (auto& p : _panelsRight) {
                    p->Render();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
    }

    //ImGui::ShowDemoWindow();

    ImGui::Render();
    ImBind::RenderDrawData(ImGui::GetDrawData());
}

void Editor::Compile()
{
    std::string output = dusk::RunCommand("cmake --build .");
    Reset();
}
