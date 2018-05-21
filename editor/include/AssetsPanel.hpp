#ifndef DUSK_ASSETS_PANEL_HPP
#define DUSK_ASSETS_PANEL_HPP

#include "Config.hpp"
#include "EditorPanel.hpp"
#include "AssetTypes.hpp"

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

    AssetType GetAssetType(std::string ext);

    void Refresh();

    struct Entry
    {
        std::string Filename;
        std::string Ext;
        AssetType Type;
        bool Directory;
    };

    std::array<char, 4096> _path;
    std::unordered_map<std::string, Entry> _entries;
};

#endif // DUSK_ASSETS_PANEL_HPP
