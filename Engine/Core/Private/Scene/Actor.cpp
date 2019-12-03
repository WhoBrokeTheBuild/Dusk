#include <Dusk/Scene/Actor.hpp>

namespace Dusk {

DUSK_CORE_API
void Actor::SetParent(Actor * parent)
{
    _parent = parent;
}

DUSK_CORE_API
void Actor::SetPosition(const vec3& position)
{
    _position = position;
}

DUSK_CORE_API
void Actor::SetOrientation(const quat& orientation)
{
    _orientation = orientation;
}

DUSK_CORE_API
void Actor::SetScale(const vec3& scale)
{
    _scale = scale;
}

DUSK_CORE_API
mat4 Actor::GetTransform() const
{
    mat4 transform = mat4(1.0f);
    transform = glm::translate(transform, GetPosition());
    transform *= glm::mat4_cast(GetOrientation());
    transform = glm::scale(transform, GetScale());
    return transform;
}

DUSK_CORE_API
vec3 Actor::GetWorldPosition() const
{
    if (_parent) {
        return _parent->GetWorldPosition() + GetPosition();
    }
    return GetPosition();
}

DUSK_CORE_API
quat Actor::GetWorldOrientation() const
{
    if (_parent) {
        return _parent->GetWorldOrientation() * GetOrientation();
    }
    return GetOrientation();
}

DUSK_CORE_API
vec3 Actor::GetWorldScale() const
{
    if (_parent) {
        return _parent->GetWorldScale() * GetScale();
    }
    return GetScale();
}

DUSK_CORE_API
mat4 Actor::GetWorldTransform() const
{
    if (_parent) {
        return _parent->GetWorldTransform() * GetTransform();
    }
    return GetTransform();
}

} // namespace Dusk