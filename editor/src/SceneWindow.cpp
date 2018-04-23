#include "SceneWindow.hpp"

#include "Editor.hpp"

SceneWindow::SceneWindow(Editor * editor, bool shown)
    : EditorWindow(editor, shown)
{
    Reset();

    _newActorId[0] = '\0';
}

void SceneWindow::DoReset()
{
}

void SceneWindow::DoApply()
{
}

void SceneWindow::DoRender()
{
    dusk::Scene * scene = GetEditor()->GetActiveScene();
    if (!scene) return;

    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin("Scene", &_shown)) {
        ImGui::BeginChild("Scroll", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, 0);

        ImGui::Text("Actors");
        ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);
        if (ImGui::Button("Add")) {
            ImGui::OpenPopup("AddActor");
            ImGui::SetKeyboardFocusHere(0);
        }
        if (ImGui::BeginPopup("AddActor")) {
            bool added = false;
            added |= ImGui::InputText("ID", _newActorId.data(), _newActorId.size(), ImGuiInputTextFlags_EnterReturnsTrue);
            added |= ImGui::Button("Done");

            if (added) {
                scene->AddActor(std::make_unique<dusk::Actor>(std::string(_newActorId.data()), scene));
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        const auto& actors = scene->GetAllActors();

        unsigned int index = 0;
        for (const auto& actor : actors) {
            const auto& id = actor->GetId();

            ImGui::Text("ID: %s", id.c_str());
            ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);

            std::string editId = "Edit##" + id;
            if (ImGui::Button(editId.c_str())) {
                AddPopup(std::make_unique<ActorWindow>(actor, this));
            }

            if (index != actors.size() - 1) {
                ImGui::Separator();
            }
            ++index;
        }

        ImGui::EndChild();
        ImGui::End();
    }
}
