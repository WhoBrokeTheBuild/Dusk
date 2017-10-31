#include "dusk/scene/Scene.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

namespace dusk {

Scene::Scene()
    : _actors()
    , _actorsByTag()
    , _tagsByActor()
{ }

Actor * Scene::AddActor(std::unique_ptr<Actor> actor, const std::vector<std::string>& tags /*= std::vector<std::string>()*/)
{
    _actors.push_back(std::move(actor));
    for (const auto& tag : tags)
    {
        TagActor(tag, actor.get());
    }
    return actor.get();
}

bool Scene::TagActor(const std::string& tag, Actor * actor)
{
    if (_tagsByActor.find(actor) == _tagsByActor.end())
    {
        // Not one of our Actors
        return false;
    }

    if (_actorsByTag.find(tag) == _actorsByTag.end())
    {
        // New tag
        _actorsByTag[tag] = std::vector<Actor *>();
    }

    const auto& it = std::find(_actorsByTag[tag].begin(), _actorsByTag[tag].end(), actor);
    if (it == _actorsByTag[tag].end())
    {
        _actorsByTag[tag].push_back(actor);
        _tagsByActor[actor].push_back(tag);
    }

    return true;
}

bool Scene::UntagActor(const std::string& tag, Actor * actor)
{
    if (_tagsByActor.find(actor) == _tagsByActor.end())
    {
        // Not one of our Actors
        return false;
    }

    if (_actorsByTag.find(tag) == _actorsByTag.end())
    {
        // Tag doesn't exist
        return false;
    }

    const auto& it = std::find(_actorsByTag[tag].begin(), _actorsByTag[tag].end(), actor);
    if (it == _actorsByTag[tag].end())
    {
        // Actor doesn't have that tag
        return false;
    }
    _actorsByTag[tag].erase(it);

    const auto& it2 = std::find(_tagsByActor[actor].begin(), _tagsByActor[actor].end(), tag);
    if (it2 == _tagsByActor[actor].end())
    {
        // Shouldn't happen, but sure
        return false;
    }
    _tagsByActor[actor].erase(it2);

    return true;
}

Actor * Scene::GetActorWithTag(const std::string& tag)
{
    if (_actorsByTag.find(tag) == _actorsByTag.end())
    {
        // Tag doesn't exist
        return nullptr;
    }

    if (_actorsByTag[tag].empty())
    {
        // No Actors to return
        return nullptr;
    }

    return _actorsByTag[tag].front();
}

std::vector<Actor *> Scene::GetActorsWithTag(const std::string& tag)
{
    if (_actorsByTag.find(tag) == _actorsByTag.end())
    {
        // Tag doesn't exist
        return std::vector<Actor *>();
    }

    return _actorsByTag[tag];
}

std::vector<std::string> Scene::GetActorTags(Actor * actor)
{
    if (_tagsByActor.find(actor) == _tagsByActor.end())
    {
        // Not one of our Actors
        return std::vector<std::string>();
    }

    return _tagsByActor[actor];
}

} // namespace dusk
