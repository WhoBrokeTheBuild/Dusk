#include "dusk/editor/AssetsPanel.hpp"
#include "dusk/editor/Editor.hpp"

#include <dusk/core/Benchmark.hpp>

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
    //if (ImGui::AddTab("Assets")) {
        ImGui::Text("Path: assets/");
        ImGui::SameLine();
        if (ImGui::InputText("##path", _path.data(), _path.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
            Refresh();
        }
        ImGui::SameLine();
        if (ImGui::Button("Refresh")) {
            Refresh();
        }

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
                ImGui::Text(e.first.c_str());
            }
        }
    //}
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

            Entry e;
            e.Directory = ent.IsDirectory();
            _entries.emplace(filename, e);
        }
    }
}
