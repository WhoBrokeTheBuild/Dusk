#include "ShadersPanel.hpp"
#include "Editor.hpp"

ShadersPanel::ShadersPanel(Editor * editor)
    : EditorPanel(editor)
{
    Reset();
}

void ShadersPanel::DoReset()
{
}

void ShadersPanel::DoApply()
{
}

void ShadersPanel::DoRender()
{
    static auto& shaders = GetEditor()->GetShaders();

    if (ImGui::AddTab("Shaders"))
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

            ImGui::Text("Uniform Locations:");
            for (const auto& uniform : uniforms)
            {
                ImGui::Text("  %s: %d", uniform.first.c_str(), uniform.second.Location);
            }

            ImGui::Text("Attribute Locations:");
            for (const auto& attribute : attributes)
            {
                ImGui::Text("  %s: %u", attribute.first.c_str(), attribute.second);
            }

            ImGui::Separator();
        }
        ImGui::EndChild();
    }
}
