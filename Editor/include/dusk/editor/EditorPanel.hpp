#ifndef DUSK_EDITOR_PANEL_HPP
#define DUSK_EDITOR_PANEL_HPP

#include "EditorConfig.hpp"

class Editor;

class EditorPanel : public dusk::ICallbackHost
{
public:

    EditorPanel(Editor * editor)
        : _editor(editor)
    { }

    virtual ~EditorPanel() = default;

    void Render();

    void Reset()
    {
        DoReset();
    }

    void Apply()
    {
        DoApply();
    }

    void Save()
    {
        DoApply();
    }

    Editor * GetEditor() { return _editor; }

protected:

    virtual void DoReset() { }
    virtual void DoApply() { }
    virtual void DoRender() = 0;

private:

    Editor * _editor;

};

#endif // DUSK_EDITOR_PANEL_HPP
