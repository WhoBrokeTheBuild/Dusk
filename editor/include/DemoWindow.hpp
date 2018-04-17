#ifndef DUSK_DEMO_WINDOW_HPP
#define DUSK_DEMO_WINDOW_HPP

#include "Config.hpp"
#include "EditorWindow.hpp"

class Editor;

class DemoWindow : public EditorWindow
{
public:

    DemoWindow(Editor * editor, bool shown = false)
        : EditorWindow(editor, shown)
    {
        Reset();
    }

protected:

    virtual void DoRender() override
    {
        ImGui::ShowTestWindow();
    }

private:

};

#endif // DUSK_DEMO_WINDOW_HPP
