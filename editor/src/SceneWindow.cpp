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

void RenderActorTree(dusk::Actor * parent, int depth = 0)
{
    const auto& actors = parent->GetChildren();
    std::string leftPad = "";
    for (int i = 0; i < depth; ++i) {
        leftPad += "  ";
    }

    unsigned int index = 0;
    for (const auto& actor : actors) {
        const auto& id = actor->GetId();
        const auto& type = parent->GetChildType(id);

        ImGui::Text("%sID: %s", leftPad.c_str(), id.c_str());
        ImGui::Text("%sType: %s", leftPad.c_str(), type.name());

        if (!actor->GetChildren().empty()) {
            ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_FirstUseEver);
            if (ImGui::TreeNode(id.c_str(), "Children"))
            {
                RenderActorTree(actor, depth + 1);
                ImGui::TreePop();
            }
        }

        if (index != actors.size() - 1) {
            ImGui::Separator();
        }
        ++index;
    }
}

void SceneWindow::DoRender()
{
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin("Scene", &_shown)) {
        ImGui::BeginChild("Scroll", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, 0);

        //const auto& subclasses = dusk::Actor::GetSubclasses();
        //ImGui::Text("Actor Subclasses");
        //for (const auto& pair : subclasses) {
        //    ImGui::Text("%s", pair.first.c_str());
        //}

        RenderActorTree(GetEditor()->GetActiveScene());

        ImGui::EndChild();

        ImGui::End();
    }
}
