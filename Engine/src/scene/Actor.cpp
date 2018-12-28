#include "dusk/scene/Actor.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/scene/Scene.hpp>

namespace dusk {

Actor::Actor(std::string id, Scene * scene)
    : _scene(scene)
    , _id(id)
    , _transform(1)
    , _position(0)
    , _rotation(0)
    , _scale(1)
{
}

void Actor::SetPosition(const glm::vec3& pos)
{
    _position = pos;
}

void Actor::SetRotation(const glm::vec3& rot)
{
    _rotation = rot;
}

void Actor::SetScale(const glm::vec3& scale)
{
    _scale = scale;
}

glm::mat4 Actor::GetTransform()
{
    //_transform = (_parent ? _parent->GetTransform() : glm::mat4());
    _transform = glm::mat4();
    _transform = glm::translate(_transform, _position);
    _transform = glm::rotate(_transform, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    _transform = glm::rotate(_transform, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    _transform = glm::rotate(_transform, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    _transform = glm::scale(_transform, _scale);

    return _transform;
}

void Actor::AddComponent(std::unique_ptr<IComponent>&& ptr)
{
    _rawComponents.push_back(ptr.get());
    _components.push_back(std::move(ptr));
}

std::vector<IComponent *> Actor::GetComponents() const
{
    return _rawComponents;
}

} // namespace dusk
