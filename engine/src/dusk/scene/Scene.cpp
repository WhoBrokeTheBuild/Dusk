#include "dusk/scene/Scene.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

#include <fstream>

namespace dusk {

Scene::Scene(std::string id)
    : _id(id)
{ }

void Scene::Serialize(nlohmann::json& data)
{

}

void Scene::Deserialize(nlohmann::json& data)
{

}

void Scene::Start()
{
    TrackCallback(App::Inst()->OnUpdate.AddMember<Scene>(this, &Scene::Update));
    TrackCallback(App::Inst()->OnRender.AddPassthrough(&OnRender));
}

void Scene::Stop()
{
    ClearCallbacks();
}

bool Scene::Load(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        return false;
    }

    nlohmann::json data;
    file >> data;

    Deserialize(data);

    return true;
}

bool Scene::Save(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file) {
        return false;
    }

    nlohmann::json data;
    Serialize(data);

    file << data;

    return true;
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
