#ifndef DUSK_COMPONENT_PANEL_HPP
#define DUSK_COMPONENT_PANEL_HPP

#include "Config.hpp"
#include "EditorPanel.hpp"

class Editor;

class ComponentPanel : public EditorPanel
{
public:

    ComponentPanel(Editor * editor);

protected:

    virtual void DoReset() override;
    virtual void DoApply() override;
    virtual void DoRender() override;

private:

    dusk::IComponent * _component = nullptr;

};

#endif // DUSK_COMPONENT_PANEL_HPP
