#ifndef DUSK_SCENE_HPP
#define DUSK_SCENE_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Context.hpp>
#include <dusk/scene/Actor.hpp>
#include <dusk/scene/Camera.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace dusk {

class Scene
{
public:

    DISALLOW_COPY_AND_ASSIGN(Scene);

    Scene();
    virtual ~Scene() = default;

    inline Actor * AddActor(Actor * actor, const std::vector<std::string>& tags = std::vector<std::string>())
    {
        return AddActor(std::unique_ptr<Actor>(actor), tags);
    }

    Actor * AddActor(std::unique_ptr<Actor> actor, const std::vector<std::string>& tags = std::vector<std::string>());

    bool TagActor(const std::string& tag, Actor * actor);
    bool UntagActor(const std::string& tag, Actor * actor);

    Actor * GetActorWithTag(const std::string& tag);
    std::vector<Actor *> GetActorsWithTag(const std::string& tag);
    std::vector<std::string> GetActorTags(Actor * actor);

private:

    std::vector<std::unique_ptr<Actor>> _actors;

    std::unordered_map<std::string, std::vector<Actor *>> _actorsByTag;
    std::unordered_map<Actor *, std::vector<std::string>> _tagsByActor;

}; // class Scene

} // namespace dusk

#endif // DUSK_SCENE_HPP
