#ifndef DUSK_EDITOR_HPP
#define DUSK_EDITOR_HPP

#include "Config.hpp"
#include <vector>
#include <unordered_map>

#include "EditorWindow.hpp"
#include "ShadersWindow.hpp"
#include "SettingsWindow.hpp"
#include "SceneWindow.hpp"

class Editor : public dusk::App
{
public:

    Editor(int argc, char ** argv);

    virtual ~Editor()
    {
        ImGui_ImplSdlGL3_Shutdown();
    }

    std::unordered_map<std::string, std::unique_ptr<dusk::Scene>>& GetScenes() { return dusk::App::GetScenes(); }
    std::vector<std::unique_ptr<dusk::Shader>>& GetShaders() { return dusk::App::GetShaders(); }

protected:

    virtual void Reset() override;

    virtual void Update() override
    {
        dusk::App::Update();
    }

    virtual void Render() override;

    virtual void ProcessSdlEvent(SDL_Event * evt) override
    {
        ImGui_ImplSdlGL3_ProcessEvent(evt);
        dusk::App::ProcessSdlEvent(evt);
    }

private:

    std::unordered_map<std::string, std::unique_ptr<EditorWindow>> _windows;
};

#endif // DUSK_EDITOR_HPP
