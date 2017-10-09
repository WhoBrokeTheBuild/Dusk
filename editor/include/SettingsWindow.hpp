#ifndef DUSK_SETTINGS_WINDOW_HPP
#define DUSK_SETTINGS_WINDOW_HPP

#include "EditorWindow.hpp"
#include <array>

class SettingsWindow : public EditorWindow
{
public:

    SettingsWindow(Editor * editor, bool shown = false);

protected:

    virtual void DoReset() override;
    virtual void DoApply() override;
    virtual void DoRender() override;

private:

    std::array<char, 128> _windowTitle;
    glm::ivec2 _windowSize;

    bool _windowSectionShown = true;

};

#endif // DUSK_SETTINGS_WINDOW_HPP
