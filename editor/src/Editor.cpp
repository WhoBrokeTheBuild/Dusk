#include "Editor.hpp"

Editor::Editor(int argc, char ** argv)
    : dusk::App(argc, argv)
{
    DuskLogInfo("Running from the Editor");

    ImGui::CreateContext();

    _windows.emplace("Settings", std::make_unique<SettingsWindow>(this, false));
    _windows.emplace("Shaders", std::make_unique<ShadersWindow>(this, false));
    _windows.emplace("Scene", std::make_unique<SceneWindow>(this, true));
    _windows.emplace("ImGui Demo", std::make_unique<DemoWindow>(this, false));

    ImBind::Init(GetSdlWindow());

    TrackCallback(OnKeyPress.AddStatic([this](dusk::Key key, dusk::Flags flags) {
        if (key == SDLK_SPACE && (flags & KMOD_CTRL) > 0) {
            TogglePlay();
        }
    }));

    TrackCallback(OnWindowResize.AddStatic([this](glm::ivec2 size) {
        InitFramebuffer();
    }));

    InitFramebuffer();
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
}

void Editor::Render()
{
    ImBind::NewFrame();

    if (!GetActiveScene()) {
        SetActiveScene(AddScene(std::make_unique<dusk::Scene>("default")));
    }

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

        if (ImGui::BeginMenu("Windows"))
        {
            if (ImGui::MenuItem("Settings", "", _windows["Settings"]->IsShown()))
            {
                _windows["Settings"]->Toggle();
            }

            if (ImGui::MenuItem("Shaders", "", _windows["Shaders"]->IsShown()))
            {
                _windows["Shaders"]->Toggle();
            }

            if (ImGui::MenuItem("Scene", "", _windows["Scene"]->IsShown()))
            {
                _windows["Scene"]->Toggle();
            }

            if (ImGui::MenuItem("ImGui Demo", "", _windows["ImGui Demo"]->IsShown()))
            {
                _windows["ImGui Demo"]->Toggle();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Program"))
        {
            if (ImGui::MenuItem("Compile", ""))
            {
                dusk::RunCommand("cmake --build .");
                Reset();
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

        ImGui::SameLine((float)winSize.x - 150, 0.0f);
        ImGui::Text("%.2f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

    auto size = GetWindowSize();

    ImGui::SetNextWindowSize(ImVec2(size.x * 0.2, size.y - 16));
    ImGui::SetNextWindowPos(ImVec2(0, 16));
    if (ImGui::Begin("panel_left", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

        ImGui::Text("Hello, Left!");

        ImGui::End();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::SetNextWindowSize(ImVec2(size.x * 0.6, size.y * 0.6));
    ImGui::SetNextWindowPos(ImVec2(size.x * 0.2, 16));
    if (ImGui::Begin("panel_center", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

        ImGui::Image((void *)_glTexBuf, ImVec2(size.x * 0.6, size.y * 0.6), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }

    ImGui::PopStyleVar();

    ImGui::SetNextWindowSize(ImVec2(size.x * 0.6, size.y * 0.4 - 16));
    ImGui::SetNextWindowPos(ImVec2(size.x * 0.2, size.y * 0.6 + 16));
    if (ImGui::Begin("panel_bottom", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

        ImGui::Text("Hello, Bottom!");

        ImGui::End();
    }

    ImGui::SetNextWindowSize(ImVec2(size.x * 0.2, size.y - 16));
    ImGui::SetNextWindowPos(ImVec2(size.x * 0.8, 16));
    if (ImGui::Begin("panel_right", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

        ImGui::Text("Hello, Right!");

        ImGui::End();
    }

    ImGui::PopStyleVar();


    //ImGui::ShowTestWindow();
    //for (auto& window : _windows)
    //{
    //    window.second->Render();
    //}

    ImGui::Render();
    ImBind::RenderDrawData(ImGui::GetDrawData());
}
