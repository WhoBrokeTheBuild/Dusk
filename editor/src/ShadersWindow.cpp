#include "ShadersWindow.hpp"
#include "Editor.hpp"

ShadersWindow::ShadersWindow(Editor * editor, bool shown)
    : EditorWindow(editor, shown)
{
    Reset();
}

void ShadersWindow::DoReset()
{
}

void ShadersWindow::DoApply()
{
}

void ShadersWindow::DoRender()
{
    static auto& shaders = GetEditor()->GetShaders();

    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin("Shaders", &_shown))
    {
        ImGui::BeginChild("Scroll", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, 0);
        for (const auto& shader : shaders)
        {
            const auto& filenames = shader->GetFilenames();
            const auto& uniforms = shader->GetAllUniforms();
            const auto& attributes = shader->GetAllAttributes();

            ImGui::Text("Files:");
            for (const auto& filename : filenames)
            {
                ImGui::Text("  %s", filename.c_str());
            }

            ImGui::Text("Uniforms:");
            for (const auto& uniform : uniforms)
            {
                ImGui::Text("  %s: %d", uniform.first.c_str(), uniform.second.Location);
            }

            ImGui::Text("Attributes:");
            for (const auto& attribute : attributes)
            {
                ImGui::Text("  %s: %u", attribute.first.c_str(), attribute.second);
            }

            ImGui::Separator();
        }
        ImGui::EndChild();

        ImGui::End();
    }
}
