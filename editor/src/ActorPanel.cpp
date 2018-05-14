#include "ActorPanel.hpp"
#include "Editor.hpp"

ActorPanel::ActorPanel(Editor * editor)
    : EditorPanel(editor)
{
}

void ActorPanel::SetActor(dusk::Actor * actor) {
    _actor = actor;
    Reset();
}

void ActorPanel::DoReset()
{
    if (!_actor) return;

    _position = _actor->GetPosition();
    _rotation = _actor->GetRotation();
    _scale = _actor->GetScale();
}

void ActorPanel::DoApply()
{
    if (!_actor) return;

    _actor->SetPosition(_position);
    _actor->SetRotation(_rotation);
    _actor->SetScale(_scale);
}

void ActorPanel::DoRender()
{
    if (ImGui::AddTab("Actor")) {
        if (!_actor) {
            ImGui::Text("No Actor Selected");
            return;
        }

        const auto& id = _actor->GetId();
        std::string windowTitle = "Actor - " + id;

        if (GetEditor()->IsPlaying()) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

            Reset();
        }

        ImGui::Text("ID: %s", id.c_str());

        ImGui::SameLine(ImGui::GetWindowWidth() - 150.0f);
        if (ImGui::Button("Reset")) Reset();
        ImGui::SameLine();
        if (ImGui::Button("Apply")) Apply();
        ImGui::SameLine();
        if (ImGui::Button("Save")) Save();

        ImGui::Spacing();

        bool apply = false;

        apply |= ImGui::InputFloat3("Position", (float *)&_position, -1, ImGuiInputTextFlags_EnterReturnsTrue);
        apply |= ImGui::InputFloat3("Rotation", (float *)&_rotation, -1, ImGuiInputTextFlags_EnterReturnsTrue);
        apply |= ImGui::InputFloat3("Scale", (float *)&_scale, -1, ImGuiInputTextFlags_EnterReturnsTrue);

        if (apply) Apply();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        // TODO: Improve

        ImGui::Text("Components");
        ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);

        std::string addCompId = "Add##" + id;
        if (ImGui::Button(addCompId.c_str())) {
            //
        }

        ImGui::Spacing();

        const auto& components = _actor->GetComponents();
        for (const auto& c : components) {
            ImGui::Indent();

            if (dusk::MeshComponent * mc = dynamic_cast<dusk::MeshComponent *>(c)) {
                dusk::Mesh * mesh = mc->GetMesh();

                ImGui::Text("Type: MeshComponent");

                std::string filename = "File: " + mesh->GetFilename();
                ImGui::Text("%s", filename.c_str());
            }

            ImGui::Unindent();
        }

        if (GetEditor()->IsPlaying()) {
            ImGui::PopStyleVar();
            ImGui::PopItemFlag();
        }
    }
}
