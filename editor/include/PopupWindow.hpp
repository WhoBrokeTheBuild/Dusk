#ifndef DUSK_POPUP_WINDOW_HPP
#define DUSK_POPUP_WINDOW_HPP

#include "Config.hpp"

class EditorWindow;

class PopupWindow : public dusk::ICallbackHost
{
public:

    PopupWindow(EditorWindow * parent)
        : _parent(parent)
    { }

    virtual ~PopupWindow() { Close(); };

    void Render()
    {
        if (!_shown) Close();

        DoRender();
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
        Close();
    }

    void Close();

protected:

    EditorWindow * GetParent() { return _parent; }

    virtual void DoReset() { }
    virtual void DoApply() { }
    virtual void DoRender() = 0;

    bool _shown = true;

private:

    EditorWindow * _parent;

};

#endif // DUSK_POPUP_WINDOW_HPP
