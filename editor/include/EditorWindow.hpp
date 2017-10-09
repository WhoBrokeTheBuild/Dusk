#ifndef DUSK_EDITOR_WINDOW_HPP
#define DUSK_EDITOR_WINDOW_HPP

#include "Config.hpp"

class Editor;

class EditorWindow
{
public:

    EditorWindow(Editor * editor, bool shown = false)
        : _shown(shown)
        , _editor(editor)
    { }

    virtual ~EditorWindow() = default;

    void Render()
    {
        if (_shown) DoRender();
    }

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
        Hide();
    }

    void Show() { _shown = true; }
    void Hide() { _shown = false; }
    void Toggle() { _shown = !_shown; }

    bool& IsShown() { return _shown; }

protected:

    Editor * GetEditor() { return _editor; }

    virtual void DoReset() { }
    virtual void DoApply() { }
    virtual void DoRender() = 0;

    bool _shown;

private:

    Editor * _editor;

};

#endif // DUSK_EDITOR_WINDOW_HPP
