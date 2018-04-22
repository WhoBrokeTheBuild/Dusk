#include "SceneWindow.hpp"

#include "Editor.hpp"

SceneWindow::SceneWindow(Editor * editor, bool shown)
    : EditorWindow(editor, shown)
{
    Reset();
}

void SceneWindow::DoReset()
{
}

void SceneWindow::DoApply()
{
}

void SceneWindow::DoRender()
{
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin("Scene", &_shown)) {
        ImGui::BeginChild("Scroll", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, 0);

        if (GetEditor()->GetActiveScene()) {
            const auto& actors = GetEditor()->GetActiveScene()->GetAllActors();

            unsigned int index = 0;
            for (const auto& actor : actors) {
                const auto& id = actor->GetId();

                ImGui::Text("ID: %s", id.c_str());
                ImGui::SameLine(ImGui::GetWindowWidth() - 40);

                std::string editId = "Edit##" + id;
                if (ImGui::Button(editId.c_str())) {
                    AddPopup(std::make_unique<ActorWindow>(actor, this));
                }

                if (index != actors.size() - 1) {
                    ImGui::Separator();
                }
                ++index;
            }    
        }

        ImGui::EndChild();
        ImGui::End();
    }
}
