#include "dusk/scene/Scene.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/core/Filesystem.hpp>
#include <dusk/core/Benchmark.hpp>

#include <fstream>

namespace dusk {

std::unordered_map<std::string, Scene::TypeFunc> Scene::_TypeFuncs;

void Scene::RegisterType(const std::string& type, TypeFunc func)
{
    DuskLogInfo("Registering Scene Type '%s'", type.c_str());
    if (_TypeFuncs.find(type) == _TypeFuncs.end()) {
        _TypeFuncs[type] = func;
    }
}

Scene * Scene::CreateInstanceOfType(const std::string& type, const std::string& id, const std::string& filename)
{
    if (_TypeFuncs.find(type) == _TypeFuncs.end()) {
        DuskLogWarn("Failed to create Scene of type '%s'", type.c_str());
        return nullptr;
    }
    return _TypeFuncs[type](id, filename);
}

Scene::Scene(const std::string& id, const std::string& filename /* = "" */)
    : _id(id)
    , _filename(filename)
{
    if (GetFilename().empty()) {
        _filename = "data/" + id + ".duskscene";
    }

    DuskLogInfo("Scene Filename: %s", GetFilename().c_str());
}

void Scene::Serialize(nlohmann::json& data)
{
    data["DefaultShader"] = _defaultShader;
    data["DefaultCamera"] = _defaultCamera;

    data["Actors"] = nlohmann::json::array();
    for (auto& actor : _actors) {
        if (actor->GetId()[0] == '_') continue;

        data["Actors"].push_back(nlohmann::json::object());
        actor->Serialize(data["Actors"].back());
    }

    data["Cameras"] = nlohmann::json::array();
    for (auto& camera : _cameras) {
        if (camera->GetId()[0] == '_') continue;

        data["Cameras"].push_back(nlohmann::json::object());
        camera->Serialize(data["Cameras"].back());
    }
}

void Scene::Deserialize(nlohmann::json& data)
{
    App * app = App::Inst();

    if (data.find("Actors") != data.end()) {
        for (auto& actor : data["Actors"]) {
            Actor * tmp = AddActor(std::make_unique<Actor>(actor["Id"], this));
            tmp->Deserialize(actor);
        }
    }

    if (data.find("Cameras") != data.end()) {
        for (auto& camera : data["Cameras"]) {
            Camera * tmp = AddCamera(std::make_unique<Camera>(camera["Id"], this));
            tmp->Deserialize(camera);
        }
    }

    if (data.find("DefaultShader") != data.end()) {
        _defaultShader = data["DefaultShader"];
        app->GetRenderContext().CurrentShader = app->GetShader(_defaultShader);
    }

    if (data.find("DefaultCamera") != data.end()) {
        _defaultCamera = data["DefaultCamera"];
        app->GetRenderContext().CurrentCamera = GetCamera(_defaultCamera);
    }
}

void Scene::Start()
{
    if (!IsLoaded()) {
        Load();
    }

    TrackCallback(App::Inst()->OnUpdate.AddMember<Scene>(this, &Scene::Update));
    TrackCallback(App::Inst()->OnRender.AddPassthrough(&OnRender));
}

void Scene::Stop()
{
    ClearCallbacks();
}

bool Scene::Load()
{
    // TODO: Move to Reset()
    _cameras.clear();
    _actors.clear();
    _actorsByTag.clear();
    _toAdd.clear();
    _toRemove.clear();

    App * app = App::Inst();
    Path fullPath = Path(app->GetProjectDir()) + Path(GetFilename());

    std::ifstream file(fullPath.GetStr());
    if (!file) {
        DuskLogWarn("Failed to load scene from '%s'", GetFilename().c_str());
        // Prevent endless loading of a file that doesn't exist
        _loaded = true;
        return false;
    }

    nlohmann::json data;
    file >> data;

    Deserialize(data);

    _loaded = true;
    return true;
}

bool Scene::Save()
{
    App * app = App::Inst();
    Path fullPath = Path(app->GetProjectDir()) + Path(GetFilename());

    std::ofstream file(fullPath.GetStr());
    if (!file) {
        DuskLogWarn("Failed to save scene to '%s'", GetFilename().c_str());
        return false;
    }

    nlohmann::json data;
    Serialize(data);

    file << std::setw(4) << data;

    return true;
}

Camera * Scene::AddCamera(std::unique_ptr<Camera>&& ptr)
{
    Camera * tmp = ptr.get();
    _cameras.push_back(std::move(ptr));
    return tmp;
}

Camera * Scene::GetCamera(const std::string& id)
{
    auto it = std::find_if(_cameras.begin(), _cameras.end(), [&](std::unique_ptr<Camera>& ptr) {
        return (ptr->GetId() == id);
    });

    if (it != _cameras.end()) {
        return (*it).get();
    }

    return nullptr;
}

void Scene::RemoveActor(Actor * actor)
{
    const auto& tags = actor->GetTags();
    for (const auto& tag : tags) {
        UntagActor(actor, tag);
    }

    _toRemove.push_back(actor);
}

void Scene::TagActor(Actor * actor, std::string tag, bool propagate /*= true*/)
{
    const auto& tags = actor->GetTags();
    if (std::find(tags.begin(), tags.end(), tag) != tags.end()) {
        return;
    }

    if (_actorsByTag.find(tag) == _actorsByTag.end()) {
        _actorsByTag[tag] = std::vector<Actor *>();
    }

    _actorsByTag[tag].push_back(actor);
    if (propagate) {
        actor->AddTag(tag, false);
    }
}

bool Scene::UntagActor(Actor * actor, std::string tag, bool propagate /*= true*/)
{
    const auto& tags = actor->GetTags();
    if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
        return false;
    }

    if (propagate) {
        actor->RemoveTag(tag, false);
    }

    if (_actorsByTag.find(tag) == _actorsByTag.end()) {
        return false;
    }

    auto it = std::find(_actorsByTag[tag].begin(), _actorsByTag[tag].end(), actor);
    if (it == _actorsByTag[tag].end()) {
        return false;
    }

    _actorsByTag[tag].erase(it);

    if (_actorsByTag[tag].empty()) {
        _actorsByTag.erase(_actorsByTag.find(tag));
    }

    return true;
}

std::vector<Actor *> Scene::GetAllActors()
{
    std::vector<Actor *> tmp;

    for (auto& actor : _actors) {
        tmp.push_back(actor.get());
    }

    return tmp;
}

std::vector<Actor *> Scene::GetActorsWithTag(std::string tag)
{
    if (_actorsByTag.find(tag) == _actorsByTag.end()) {
        return std::vector<Actor *>();
    }

    return _actorsByTag[tag];
}

Actor * Scene::GetFirstActorWithTag(std::string tag)
{
    if (_actorsByTag.find(tag) == _actorsByTag.end()) {
        return nullptr;
    }

    return _actorsByTag[tag].front();
}

void Scene::Update(UpdateContext& ctx)
{
    OnUpdate.Call(ctx);

    for (auto& actor : _toAdd) {
        _actors.push_back(std::move(actor));
    }
    _toAdd.clear();

    for (auto& actor : _toRemove) {
        auto it = std::find_if(_actors.begin(), _actors.end(), [&](std::unique_ptr<Actor>& ptr) {
            return (ptr.get() == actor);
        });

        if (it != _actors.end()) {
            _actors.erase(it);
        }
    }
    _toRemove.clear();
}

} // namespace dusk
