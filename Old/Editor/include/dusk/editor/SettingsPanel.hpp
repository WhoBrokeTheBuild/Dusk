#ifndef DUSK_SETTINGS_PANEL_HPP
#define DUSK_SETTINGS_PANEL_HPP

#include "EditorPanel.hpp"
#include <array>

class SettingsPanel : public EditorPanel
{
public:

    SettingsPanel(Editor * editor);

protected:

    virtual void DoReset() override;
    virtual void DoApply() override;
    virtual void DoRender() override;

private:

    std::array<char, 128> _windowTitle;
    std::array<char, 128> _startScene;

};

#endif // DUSK_SETTINGS_PANEL_HPP
