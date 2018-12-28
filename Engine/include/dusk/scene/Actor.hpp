#ifndef DUSK_ACTOR_HPP
#define DUSK_ACTOR_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Context.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/scene/IComponent.hpp>

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <memory>
using std::unique_ptr;

namespace dusk {

class Scene;

class Actor
{
public:

    DISALLOW_COPY_AND_ASSIGN(Actor)

    Actor(string id, Scene * scene);
    virtual ~Actor() = default;

    inline string GetId() { return _id; }

    void SetPosition(const glm::vec3& pos);
    inline glm::vec3 GetPosition() const { return _position; }

    void SetRotation(const glm::vec3& rot);
    inline glm::vec3 GetRotation() const { return _rotation; }

    void SetScale(const glm::vec3& scale);
    inline glm::vec3 GetScale() const { return _scale; }

    glm::mat4 GetTransform();

    void AddComponent(unique_ptr<IComponent>&& ptr);

    vector<IComponent *> GetComponents() const;

private:

    Scene * _scene;

    string _id;

    glm::mat4 _transform;
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;

    vector<unique_ptr<IComponent>> _components;

    vector<IComponent *> _rawComponents;

}; // class Actor

} // namespace dusk

#endif // DUSK_ACTOR_HPP
