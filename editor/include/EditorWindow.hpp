#ifndef DUSK_EDITOR_WINDOW_HPP
#define DUSK_EDITOR_WINDOW_HPP

#include "Config.hpp"

class Editor;
class PopupWindow;

class EditorWindow : public dusk::ICallbackHost
{
public:

    EditorWindow(Editor * editor, bool shown = false)
        : _shown(shown)
        , _editor(editor)
    { }

    virtual ~EditorWindow() = default;

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
        Hide();
    }

    void Show() { _shown = true; }
    void Hide() { _shown = false; }
    void Toggle() { _shown = !_shown; }

    bool& IsShown() { return _shown; }

    void AddPopup(std::unique_ptr<PopupWindow>&& popup);
    void ClosePopup(PopupWindow * window);

    Editor * GetEditor() { return _editor; }

protected:

    virtual void DoReset() { }
    virtual void DoApply() { }
    virtual void DoRender() = 0;

    bool _shown;

    std::vector<std::unique_ptr<PopupWindow>> _popupWindows;
    std::vector<PopupWindow *> _toRemove;

private:

    Editor * _editor;

};

#endif // DUSK_EDITOR_WINDOW_HPP
