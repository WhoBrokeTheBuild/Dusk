#include "dusk/editor/SettingsPanel.hpp"
#include "dusk/editor/Editor.hpp"

SettingsPanel::SettingsPanel(Editor * editor)
    : EditorPanel(editor)
{
    Reset();
}

void SettingsPanel::DoReset()
{
    const std::string& title = GetEditor()->GetWindowTitle();
    std::copy(title.c_str(), title.c_str() + title.size() + 1, _windowTitle.begin());

    _startScene[0] = '\0';
}

void SettingsPanel::DoApply()
{
    GetEditor()->SetWindowTitle(std::string(_windowTitle.data()));
}

void SettingsPanel::DoRender()
{
    //if (ImGui::AddTab("Settings"))
    {
        ImGui::InputText("Title##_window_title", _windowTitle.data(), _windowTitle.size());

        ImGui::InputText("Starting Scene##_start_scene", _startScene.data(), _startScene.size());

        ImGui::Spacing();

        if (ImGui::Button("Reset")) Reset();
        ImGui::SameLine();
        if (ImGui::Button("Apply")) Apply();
        ImGui::SameLine();
    }
}
