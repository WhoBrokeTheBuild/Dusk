#include "AssetsPanel.hpp"
#include "Editor.hpp"

#include <dusk/core/Benchmark.hpp>
#include <cctype>

AssetsPanel::AssetsPanel(Editor * editor)
    : EditorPanel(editor)
{
    _path[0] = '\0';
    Refresh();
}

void AssetsPanel::DoReset()
{
}

void AssetsPanel::DoApply()
{
}

void AssetsPanel::DoRender()
{
    if (ImGui::AddTab("Assets")) {
        ImGui::Text("Path: assets/");
        ImGui::SameLine();
        if (ImGui::InputText("##path", _path.data(), _path.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
            Refresh();
        }
        ImGui::SameLine();
        if (ImGui::Button("Refresh")) {
            Refresh();
        }

        ImGui::BeginChild("Scroll", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, 0);

        bool upDisabled = (_path[0] == '\0');
        if (upDisabled) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        if (ImGui::Button("..")) {
            std::string tmp(_path.data());

            std::vector<std::string> parts = dusk::StringSplit(dusk::Path::Sep, tmp);
            if (!parts.empty()) {
                parts.pop_back();
            }
            tmp = dusk::StringJoin(dusk::Path::Sep, parts);

            memcpy(_path.data(), tmp.data(), tmp.size() + 1);
            Refresh();
        }

        if (upDisabled) {
            ImGui::PopStyleVar();
            ImGui::PopItemFlag();
        }

        int i = 0;
        for (auto& e : _entries) {
            if (e.second.Directory) {
                if (ImGui::Button(e.first.c_str())) {
                    std::string tmp(_path.data());
                    if (!tmp.empty()) {
                        tmp += dusk::Path::Sep;
                    }

                    tmp += e.first;
                    memcpy(_path.data(), tmp.data(), tmp.size() + 1);;
                    Refresh();
                    break;
                }
            }
            else {
                ImGui::Text("%s", e.first.c_str());
                ImGui::SameLine(200);
                ImGui::Text("%s", AssetTypeNames[(int)e.second.Type].c_str());

                if (GetEditor()->IsWaitingForAsset() && GetEditor()->AssetRequestType() == e.second.Type) {
                    ImGui::SameLine(300);
                    std::stringstream label;
                    label << "Select##" << i;
                    if (ImGui::Button(label.str().c_str())) {
                        GetEditor()->OnAssetChosen.Call(e.second.Filename);
                    }
                }
            }
            ++i;
        }

        ImGui::EndChild(); // Scroll
    }
}

AssetType AssetsPanel::GetAssetType(std::string ext)
{
    for (size_t i = 0; i < ext.size(); ++i) {
        ext[i] = ::tolower((unsigned char)ext[i]);
    }

    if (ext == "glsl") {
        return AssetType::Shader;
    }
    else if (ext == "obj" || ext == "fbx" || ext == "dmf" || ext == "dmfz") {
        // TODO: Expand
        return AssetType::Model;
    }
    else if (ext == "mtl") {
        return AssetType::Material;
    }
    else if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "tga") {
        return AssetType::Texture;
    }
    else if (ext == "ttf" || ext == "otf") {
        return AssetType::Font;
    }
    else if (ext == "as") {
        return AssetType::Script;
    }
    else if (ext == "ogg" || ext == "mp3" || ext == "wav") {
        return AssetType::Audio;
    }
    else if (ext == "ogv" || ext == "mpv") {
        return AssetType::Video;
    }

    return AssetType::Unknown;
}

void AssetsPanel::Refresh()
{
    _entries.clear();
    // TODO: Alphabetize

    for (auto& path : dusk::GetAssetPaths()) {
        dusk::Path p = dusk::Path(path) + dusk::Path(_path.data());
        //DuskLogInfo("Looking in '%s'", p.GetCStr());

        for (auto ent : dusk::DirectoryIterator(p)) {
            std::string filename = ent.GetPath().GetBasename();
            if (ent.IsDirectory()) {
                filename += dusk::Path::Sep;
            }

            if (_entries.find(filename) != _entries.end()) {
                continue;
            }

            dusk::Path fullPath = dusk::Path(_path.data()) + dusk::Path(filename);

            Entry e;
            e.Filename = fullPath.GetStr();
            e.Ext = ent.GetPath().GetExt();
            e.Type = GetAssetType(e.Ext);
            e.Directory = ent.IsDirectory();
            _entries.emplace(filename, e);
        }
    }
}
