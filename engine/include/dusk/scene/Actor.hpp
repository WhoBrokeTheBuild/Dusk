#ifndef DUSK_ACTOR_HPP
#define DUSK_ACTOR_HPP

#include <dusk/Config.hpp>

#include <dusk/core/BaseClass.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/core/Log.hpp>

#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>

namespace dusk {

class Actor : public BaseClass
{
public:

    DISALLOW_COPY_AND_ASSIGN(Actor)

    Actor();
    virtual ~Actor() = default;

    virtual void Serialize(nlohmann::json& data);
    virtual void Deserialize(nlohmann::json& data);

    void SetId(const std::string& id);
    inline std::string GetId() { return _id; }

    void SetParent(Actor * pareent);
    inline Actor * GetParent() { return _parent; }

    void SetPosition(const glm::vec3& pos);
    inline glm::vec3 GetPosition() const { return _position; }

    void SetRotation(const glm::vec3& rot);
    inline glm::vec3 GetRotation() const { return _rotation; }

    void SetScale(const glm::vec3& scale);
    inline glm::vec3 GetScale() const { return _scale; }

    glm::mat4 GetTransform();

    virtual void Update(UpdateContext& ctx);
    virtual void Render(RenderContext& ctx);

    template <class T = Actor>
    T * AddChild(std::unique_ptr<T> actor, const std::string& id)
    {
        if (_childrenById.find(id) != _childrenById.end()) {
            RemoveChild(id);
        }
        if (_childrenByType.find(typeid(T)) == _childrenByType.end()) {
            _childrenByType[typeid(T)] = {};
        }
        _children.push_back(actor.get());
        _typesByChild.emplace(actor.get(), typeid(T));
        _childrenByType[typeid(T)].push_back(actor.get());
        actor->SetId(id);
        _childrenById[id] = std::move(actor);

        return dynamic_cast<T*>(_childrenById[id].get());
    }


    void RemoveChild(Actor * actor);

    void RemoveChild(const std::string& id);

    void ChangeChildId(const std::string& oldId, const std::string& newId);

    void AddChildTag(Actor * actor, const std::string& tag);

    void AddChildTag(const std::string& id, const std::string& tag);

    void RemoveChildTag(Actor * actor, const std::string& tag);

    void RemoveChildTag(const std::string& id, const std::string& tag);

    Actor * GetChild(const std::string& id);

    std::type_index GetChildType(const std::string& id) const;

    std::type_index GetChildType(Actor * actor) const;

    Actor * GetFirstChild();
    std::vector<Actor *> GetChildren();

    Actor * GetFirstChildWithTag(const std::string& tag);
    std::vector<Actor *> GetChildrenWithTag(const std::string& tag);

    template <class T>
    Actor * GetFirstChildWithType()
    {
        if (_childrenByType.find(typeid(T)) == _childrenByType.end()) {
            _childrenByType[typeid(T)] = {};
        }
        if (_childrenByType[typeid(T)].empty()) {
            return nullptr;
        }
        return _childrenByType[typeid(T)].front();
    }

    template <class T>
    std::vector<Actor *> GetChildrenWithType()
    {
        if (_childrenByType.find(typeid(T)) == _childrenByType.end()) {
            _childrenByType[typeid(T)] = {};
        }
        return _childrenByType[typeid(T)];

    }

private:

    Actor * _parent;

    std::string _id;

    glm::mat4 _transform;
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;

    std::vector<Actor *> _children;

    std::unordered_map<std::string, std::unique_ptr<Actor>> _childrenById;

    std::unordered_map<Actor *, std::vector<std::string>> _tagsByChild;

    std::unordered_map<std::string, std::vector<Actor *>> _childrenByTag;

    std::unordered_map<Actor *, std::type_index> _typesByChild;

    std::unordered_map<std::type_index, std::vector<Actor *>> _childrenByType;

}; // class Actor

} // namespace dusk

#endif // DUSK_ACTOR_HPP
