#ifndef DUSK_SHADER_WINDOW_HPP
#define DUSK_SHADER_WINDOW_HPP

#include "Config.hpp"
#include "EditorWindow.hpp"

class Editor;

class ShadersWindow : public EditorWindow
{
public:

    ShadersWindow(Editor * editor, bool shown = false);

protected:

    virtual void DoReset() override;
    virtual void DoApply() override;
    virtual void DoRender() override;

private:

};

#endif // DUSK_SHADER_WINDOW_HPP
