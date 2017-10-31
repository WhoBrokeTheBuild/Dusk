#include "dusk/scene/Actor.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/scene/Scene.hpp>

namespace dusk {

Actor::Actor(std::string name, Actor * parent /*= nullptr*/)
    : _name(name)
    , _parent(parent)
    , _transform(1)
    , _position(0)
    , _rotation(0)
    , _scale(1)
{
}

void Actor::AddChild(Actor * child)
{
    _children.push_back(child);
}

bool Actor::RemoveChild(Actor * child)
{
    const auto& it = std::find(_children.begin(), _children.end(), child);
    if (it == _children.end())
    {
        // Not our child
        return false;
    }
    _children.erase(it);
    return true;
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
    _transform = (_parent ? _parent->GetTransform() : glm::mat4());
    _transform = glm::translate(_transform, _position);
    _transform = glm::rotate(_transform, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    _transform = glm::rotate(_transform, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    _transform = glm::rotate(_transform, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    _transform = glm::scale(_transform, _scale);

    return _transform;
}

} // namespace dusk
