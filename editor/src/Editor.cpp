#include "Editor.hpp"

Editor::Editor(int argc, char ** argv)
    : dusk::App(argc, argv)
{
    DuskLogInfo("Running from the Editor");

    ImGui::CreateContext();

    _windows.emplace("Settings", std::make_unique<SettingsWindow>(this, false));
    _windows.emplace("Shaders", std::make_unique<ShadersWindow>(this, false));
    _windows.emplace("Scene", std::make_unique<SceneWindow>(this, false));
    _windows.emplace("ImGui Demo", std::make_unique<DemoWindow>(this, false));

    ImBind::Init(GetSdlWindow());
}

void Editor::Reset()
{
    dusk::App::Reset();
    TrackCallback(OnLoadConfig.AddStatic([this](std::string) {
        for (auto& win : _windows)
        {
            win.second->Reset();
        }
    }));
}

void Editor::Render()
{
    ImBind::NewFrame();

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

        ImGui::SameLine((float)winSize.x - 150, 0.0f);
        ImGui::Text("%.2f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }

    //ImGui::ShowTestWindow();
    for (auto& window : _windows)
    {
        window.second->Render();
    }

    dusk::App::Render();
    ImGui::Render();
    ImBind::RenderDrawData(ImGui::GetDrawData());
}
