#include "dusk/editor/ComponentPanel.hpp"

ComponentPanel::ComponentPanel(Editor * editor)
    : EditorPanel(editor)
{
    Reset();
}

void ComponentPanel::DoReset()
{
}

void ComponentPanel::DoApply()
{
}

void ComponentPanel::DoRender()
{
    //if (ImGui::AddTab("Component")) {
        if (!_component) {
            ImGui::Text("No Component Selected");
            return;
        }
    //}
}
