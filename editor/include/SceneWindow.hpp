#ifndef DUSK_SCENE_WINDOW_HPP
#define DUSK_SCENE_WINDOW_HPP

#include "Config.hpp"
#include "EditorWindow.hpp"
#include "ActorWindow.hpp"

class Editor;

class SceneWindow : public EditorWindow
{
public:

    SceneWindow(Editor * editor, bool shown = false);

protected:

    virtual void DoReset() override;
    virtual void DoApply() override;
    virtual void DoRender() override;

private:


};

#endif // DUSK_SCENE_WINDOW_HPP
