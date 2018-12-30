#ifndef DUSK_ACTOR_HPP
#define DUSK_ACTOR_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Context.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/scene/IComponent.hpp>

#include <memory>
#include <string>
#include <vector>

namespace dusk {

class Scene;

class Actor
{
public:

    DISALLOW_COPY_AND_ASSIGN(Actor)

    Actor(std::string id, Scene * scene);
    virtual ~Actor() = default;

    inline std::string GetId() { return _id; }

    void SetPosition(const glm::vec3& pos);
    inline glm::vec3 GetPosition() const { return _position; }

    void SetRotation(const glm::vec3& rot);
    inline glm::vec3 GetRotation() const { return _rotation; }

    void SetScale(const glm::vec3& scale);
    inline glm::vec3 GetScale() const { return _scale; }

    glm::mat4 GetTransform();

    void AddComponent(std::unique_ptr<IComponent>&& ptr);

    std::vector<IComponent *> GetComponents() const;

private:

    Scene * _scene;

    std::string _id;

    glm::mat4 _transform;
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;

    std::vector<std::unique_ptr<IComponent>> _components;

    std::vector<IComponent *> _rawComponents;

}; // class Actor

} // namespace dusk

#endif // DUSK_ACTOR_HPP
