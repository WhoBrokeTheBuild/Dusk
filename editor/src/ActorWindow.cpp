#include "ActorWindow.hpp"
#include "Editor.hpp"

ActorWindow::ActorWindow(dusk::Actor * actor, EditorWindow * parent)
    : PopupWindow(parent)
    , _actor(actor)
{
    Reset();
}

void ActorWindow::DoReset()
{
    _position = _actor->GetPosition();
    _rotation = _actor->GetRotation();
    _scale = _actor->GetScale();
}

void ActorWindow::DoApply()
{
    _actor->SetPosition(_position);
    _actor->SetRotation(_rotation);
    _actor->SetScale(_scale);
}

void ActorWindow::DoRender()
{
    const auto& id = _actor->GetId();
    std::string windowTitle = "Actor - " + id;

    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin(windowTitle.c_str(), &_shown)) {
        if (GetParent()->GetEditor()->IsPlaying()) {
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

        ImGui::InputFloat3("Position", (float *)&_position);
        ImGui::InputFloat3("Rotation", (float *)&_rotation);
        ImGui::InputFloat3("Scale", (float *)&_scale);

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

        if (GetParent()->GetEditor()->IsPlaying()) {
            ImGui::PopStyleVar();
            ImGui::PopItemFlag();
        }

        ImGui::End();
    }
}
