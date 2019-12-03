#ifndef DUSK_ACTOR_HPP
#define DUSK_ACTOR_HPP

#include <Dusk/Core.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>

namespace Dusk {

DUSK_CORE_API
static inline vec3 GetWorldUp() {
    return { 0.f, 1.f, 0.f };
}

DUSK_CORE_API 
static inline vec3 GetWorldForward() {
    return { 0.f, 0.f, -1.f };
}

class DUSK_CORE_API Actor 
{
public:

    DISALLOW_COPY_AND_ASSIGN(Actor)

    Actor() = default;

    virtual ~Actor() = default;

    void SetParent(Actor * parent);

    inline Actor * GetParent() const {
        return _parent;
    }

    void SetPosition(const vec3& position);

    inline vec3 GetPosition() const {
        return _position;
    }

    void SetOrientation(const quat& orientation);

    inline quat GetOrientation() const {
        return _orientation;
    }

    void SetScale(const vec3& scale);

    inline vec3 GetScale() const {
        return _scale;
    }

    mat4 GetTransform() const;

    vec3 GetWorldPosition() const;
    
    quat GetWorldOrientation() const;
    
    vec3 GetWorldScale() const;

    mat4 GetWorldTransform() const;

private:

    Actor * _parent = nullptr;

    vec3 _position = vec3(0.f);

    quat _orientation = quat(1.f, 0.f, 0.f, 0.f);

    vec3 _scale = vec3(1.f);

};

} // namespace Dusk

#endif // DUSK_ACTOR_HPP