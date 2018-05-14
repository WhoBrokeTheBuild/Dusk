#ifndef DUSK_EDITOR_HPP
#define DUSK_EDITOR_HPP

#include "Config.hpp"

#include "ActorPanel.hpp"
#include "ComponentPanel.hpp"
#include "EditorPanel.hpp"
#include "ShadersPanel.hpp"
#include "SettingsPanel.hpp"
#include "ScenePanel.hpp"
#include "ImBind.hpp"

#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_map>

class Editor : public dusk::App
{
public:

    Editor(int argc, char ** argv);

    virtual ~Editor();

    std::vector<std::unique_ptr<dusk::Scene>>& GetScenes() { return dusk::App::GetScenes(); }
    std::vector<std::unique_ptr<dusk::Shader>>& GetShaders() { return dusk::App::GetShaders(); }

    void Play() {
        _playing = true;
        OnPlayPause.Call(_playing);
    }

    void Pause() {
        _playing = false;
        OnPlayPause.Call(_playing);
    }

    void TogglePlay() {
        _playing = !_playing;
        OnPlayPause.Call(_playing);
    }

    bool IsPlaying() { return _playing; }

    dusk::Event<bool> OnPlayPause;

    virtual int GetSdlWindowFlags() const override {
        return App::GetSdlWindowFlags() | SDL_WINDOW_RESIZABLE;
    }

    EditorPanel * GetPanel(const std::string& id) { return _panels[id].get(); }

protected:

    virtual void Reset() override;

    virtual void Update() override
    {
        if (IsPlaying()) {
            dusk::App::Update();
        }
    }

    virtual void Render() override;

    virtual void ProcessSdlEvent(SDL_Event * evt) override
    {
        ImBind::ProcessEvent(evt);
        dusk::App::ProcessSdlEvent(evt);
    }

    void InitFramebuffer();
    void TermFramebuffer();

private:

    bool _playing = true;
    bool _fullscreen = false;

    GLuint _glTexBuf = 0;
    GLuint _glFrameBuf = 0;
    GLuint _glDepthBuf = 0;

    std::vector<float> _frameTimes;
    double _frameTimeUpdate = 0.0;

    std::unordered_map<std::string, std::function<dusk::IComponent*(void)>> _componentTypes;

    std::unordered_map<std::string, std::unique_ptr<EditorPanel>> _panels;
    std::vector<EditorPanel *> _panelsLeft;
    std::vector<EditorPanel *> _panelsBottom;
    std::vector<EditorPanel *> _panelsRight;
};

#endif // DUSK_EDITOR_HPP
