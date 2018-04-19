#ifndef DUSK_EDITOR_HPP
#define DUSK_EDITOR_HPP

#include "Config.hpp"

#include "EditorWindow.hpp"
#include "DemoWindow.hpp"
#include "ShadersWindow.hpp"
#include "SettingsWindow.hpp"
#include "SceneWindow.hpp"
#include "ImBind.hpp"

#include <vector>
#include <typeindex>
#include <unordered_map>

class Editor : public dusk::App
{
public:

    Editor(int argc, char ** argv);

    virtual ~Editor()
    {
        ImBind::Term();
        ImGui::DestroyContext();
    }

    template <class T>
    void AddActorTypeFields(std::function<void(dusk::Actor *)> callback)
    {
        _actorFieldCallbacks.emplace(typeid(T), callback);
    }

    template <class T>
    void AddShaderTypeFields(std::function<void(dusk::Shader *)> callback)
    {
        _shaderFieldCallbacks.emplace(typeid(T), callback);
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
        ImBind::ProcessEvent(evt);
        dusk::App::ProcessSdlEvent(evt);
    }

private:

    std::unordered_map<std::type_index, std::function<void(dusk::Actor *)>> _actorFieldCallbacks;
    std::unordered_map<std::type_index, std::function<void(dusk::Shader *)>> _shaderFieldCallbacks;

    std::unordered_map<std::string, std::unique_ptr<EditorWindow>> _windows;
};

#endif // DUSK_EDITOR_HPP
