#ifndef DUSK_SCENE_HPP
#define DUSK_SCENE_HPP

#include <dusk/Config.hpp>

#include <dusk/scene/Actor.hpp>
#include <dusk/core/Event.hpp>
#include <dusk/core/Context.hpp>
#include <string>

namespace dusk {

class Scene : public ICallbackHost
{
public:

    DISALLOW_COPY_AND_ASSIGN(Scene)

    Scene(std::string id);
    virtual ~Scene() = default;

    virtual void Serialize(nlohmann::json& data);
    virtual void Deserialize(nlohmann::json& data);

    inline std::string GetId() { return _id; }

    virtual void Start();
    virtual void Stop();

    bool Load(const std::string& filename);
    bool Save(const std::string& filename);

    void Update(UpdateContext& ctx);

    template <class T>
    T * AddActor(std::unique_ptr<T>&& ptr);

    void RemoveActor(Actor * actor);

    void TagActor(Actor * actor, std::string tag, bool propagate = true);
    bool UntagActor(Actor * actor, std::string tag, bool propagate = true);

    std::vector<Actor *> GetAllActors();
    std::vector<Actor *> GetActorsWithTag(std::string tag);
    Actor * GetFirstActorWithTag(std::string tag);

    Event<UpdateContext&> OnUpdate;
    Event<RenderContext&> OnRender;

private:

    std::string _id;

    std::vector<std::unique_ptr<Actor>> _actors;

    std::unordered_map<std::string, std::vector<Actor *>> _actorsByTag;

    std::vector<std::unique_ptr<Actor>> _toAdd;

    std::vector<Actor *> _toRemove;

public:

    static void Script_Init(asIScriptEngine * as)
    {
        as->RegisterObjectType("Scene", 0, asOBJ_REF | asOBJ_NOHANDLE);

        as->RegisterObjectMethod("Scene", "void OnUpdate(string)", asMETHOD(Scene, Script_OnUpdate), asCALL_THISCALL);
    }

    void Script_OnUpdate(std::string func) {
        printf("%s\n", func.c_str());
    }

}; // class Scene

template <class T>
T * Scene::AddActor(std::unique_ptr<T>&& ptr)
{
    static_assert(std::is_base_of<Actor, T>::value, "");
    _toAdd.push_back(std::move(ptr));
    return dynamic_cast<T*>(_toAdd.back().get());
}

} // namespace dusk

#endif // DUSK_SCENE_HPP
