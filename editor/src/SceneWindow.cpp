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

void RenderActorTree(dusk::Actor * parent)
{
    const auto& actors = parent->GetChildren();

    for (const auto& actor : actors)
    {
        const auto& name = actor->GetName();

        ImGui::Text("  Name: %s", name.c_str());

        ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_FirstUseEver);
        if (ImGui::CollapsingHeader("Children##%s", name.c_str()))
        {
            RenderActorTree(actor);
        }
    }
}

void SceneWindow::DoRender()
{
    const auto& actors = GetEditor()->GetActiveScene()->GetChildren();

    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin("Scene", &_shown))
    {
        ImGui::BeginChild("Scroll", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, 0);
        for (const auto& actor : actors)
        {
            const auto& name = actor->GetName();

            ImGui::Text("Name: %s", name.c_str());

            ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_FirstUseEver);
            if (ImGui::CollapsingHeader("Children##%s", name.c_str()))
            {
                RenderActorTree(actor);
            }
        }
        ImGui::EndChild();

        ImGui::End();
    }
}
