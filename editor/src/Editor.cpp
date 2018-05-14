#include "Editor.hpp"

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
        ImVec4& color = ImGui::GetStyle().Colors[i];
        std::string name = ImGui::GetStyleColorName(i);
        DuskLogInfo("Color %s (%f, %f, %f, %f)", name.c_str(), color.x, color.y, color.z, color.w);

        if (colors.find(name) != colors.end()) {
            memcpy(&ImGui::GetStyle().Colors[i], &colors[name].Value, sizeof(ImVec4));
        }
    }

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

    ImBind::Init(GetSdlWindow());

    TrackCallback(OnKeyPress.AddStatic([this](dusk::Key key, dusk::Flags flags) {
        if (key == SDLK_F5) {
            TogglePlay();
        }

        if (key == SDLK_F11) {
            _fullscreen = !_fullscreen;
        }
    }));

    TrackCallback(OnWindowResize.AddStatic([this](glm::ivec2 size) {
        InitFramebuffer();
    }));

    InitFramebuffer();

    _frameTimes.resize(20);

    DuskLogInfo("Asset Paths:");
    const auto& r = dusk::GetAssetPaths();
    for (auto& p : r) {
        DuskLogInfo("%s", p.c_str());
    }
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
    auto& ctx = GetUpdateContext();

    _frameTimeUpdate -= ctx.ElapsedTime;
    if (_frameTimeUpdate <= 0.0) {
        _frameTimeUpdate = 0.1;

        for (size_t i = 1; i < _frameTimes.size(); ++i) {
            _frameTimes[i - 1] = _frameTimes[i];
        }

        _frameTimes.pop_back();
        _frameTimes.push_back(ctx.CurrentFPS);
    }

    ImBind::NewFrame();

    if (!GetActiveScene()) {
        SetActiveScene(AddScene(std::make_unique<dusk::Scene>("default")));
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

            if (ImGui::BeginMenu("Program"))
            {
                if (ImGui::MenuItem("Compile", ""))
                {
                    std::string output = dusk::RunCommand("cmake --build .");
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

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImBind::RenderDrawData(ImGui::GetDrawData());
}
