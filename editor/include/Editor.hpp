#ifndef DUSK_EDITOR_HPP
#define DUSK_EDITOR_HPP

#include "Config.hpp"

#include "EditorPanel.hpp"
#include "ImBind.hpp"
#include "AssetTypes.hpp"

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

    void RequestAsset(AssetType type) {
        _waitingForAsset = true;
        _assetRequestType = type;

        OnAssetChosen.RemoveAllCallbacks();

        TrackCallback(OnAssetChosen.AddStatic([this](std::string) {
            _waitingForAsset = false;
        }));
    }

    bool IsWaitingForAsset() const { return _waitingForAsset; }

    AssetType AssetRequestType() const { return _assetRequestType; }

    dusk::Event<bool> OnPlayPause;

    dusk::Event<std::string> OnAssetChosen;

    EditorPanel * GetPanel(const std::string& id) { return _panels[id].get(); }

protected:

    virtual void Reset() override;

    virtual void Update() override;

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

    bool _waitingForAsset = false;
    AssetType _assetRequestType = AssetType::Unknown;

    GLuint _glTexBuf = 0;
    GLuint _glFrameBuf = 0;
    GLuint _glDepthBuf = 0;

    std::vector<float> _frameTimes;
    double _frameTimeUpdate = 0.0;

    dusk::Camera * _editorCamera = nullptr;
    dusk::Camera * _playCamera = nullptr;
    glm::vec3 _cameraDir = { 0, 0, 0 };
    const float _cameraSpeed = 0.25f;
    bool _cameraDrag = false;

    std::unordered_map<std::string, std::function<dusk::IComponent*(void)>> _componentTypes;

    std::unordered_map<std::string, std::unique_ptr<EditorPanel>> _panels;
    std::vector<EditorPanel *> _panelsLeft;
    std::vector<EditorPanel *> _panelsBottom;
    std::vector<EditorPanel *> _panelsRight;
};

#endif // DUSK_EDITOR_HPP
