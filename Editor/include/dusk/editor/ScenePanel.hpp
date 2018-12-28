#ifndef DUSK_SCENE_PANEL_HPP
#define DUSK_SCENE_PANEL_HPP

#include "EditorConfig.hpp"
#include "EditorPanel.hpp"

class ScenePanel : public EditorPanel
{
public:

    ScenePanel(Editor * editor);

protected:

    virtual void DoReset() override;
    virtual void DoApply() override;
    virtual void DoRender() override;

private:

    std::array<char, 512> _newActorId;

};

#endif // DUSK_SCENE_PANEL_HPP
