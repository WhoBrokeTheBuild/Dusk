#ifndef DUSK_ASSETS_PANEL_HPP
#define DUSK_ASSETS_PANEL_HPP

#include "Config.hpp"
#include "EditorPanel.hpp"

class Editor;

class AssetsPanel : public EditorPanel
{
public:

    AssetsPanel(Editor * editor);

protected:

    virtual void DoReset() override;
    virtual void DoApply() override;
    virtual void DoRender() override;

private:

    void Refresh();

    struct Entry
    {
        bool Directory;
    };

    std::array<char, 4096> _path;
    std::unordered_map<std::string, Entry> _entries;
};

#endif // DUSK_ASSETS_PANEL_HPP
