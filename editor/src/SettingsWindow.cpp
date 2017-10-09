#include "SettingsWindow.hpp"
#include "Editor.hpp"

SettingsWindow::SettingsWindow(Editor * editor, bool shown)
    : EditorWindow(editor, shown)
{
    Reset();
}

void SettingsWindow::DoReset()
{
    const std::string& title = GetEditor()->GetWindowTitle();
    std::copy(title.c_str(), title.c_str() + title.size() + 1, _windowTitle.begin());

    _windowSize = GetEditor()->GetWindowSize();
}

void SettingsWindow::DoApply()
{
    GetEditor()->SetWindowTitle(std::string(_windowTitle.data()));
    GetEditor()->SetWindowSize(_windowSize);
}

void SettingsWindow::DoRender()
{
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin("Settings", &_shown))
    {
        ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_FirstUseEver);
        if (ImGui::CollapsingHeader("Window", &_windowSectionShown))
        {
            const int leftSpacing = 80;
            ImGui::Text("Title: ");
            ImGui::SameLine(leftSpacing);
            ImGui::InputText("##_window_title",
                             _windowTitle.data(), _windowTitle.size());

            ImGui::Text("Size: ");
            ImGui::SameLine(leftSpacing);
            ImGui::InputInt2("##_window_size", (int *)&_windowSize);
        }

        ImGui::Spacing();

        if (ImGui::Button("Reset")) Reset();
        ImGui::SameLine();
        if (ImGui::Button("Apply")) Apply();
        ImGui::SameLine();
        if (ImGui::Button("Save")) Save();
        ImGui::SameLine();

        ImGui::End();
    }
}
