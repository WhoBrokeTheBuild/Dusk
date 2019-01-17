#include "dusk/scene/ListenerComponent.hpp"

#include <dusk/core/OpenAL.hpp>
#include <dusk/scene/Actor.hpp>

namespace dusk {

void ListenerComponent::Update(UpdateContext& ctx)
{
    if (!GetActor()) {
        return;
    }

    const auto& pos = GetActor()->GetWorldPosition();
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    
    glm::vec3 dir = glm::rotate(GetActor()->GetWorldRotation(), glm::vec3(0.f, 0.f, -1.f));
    alListener3f(AL_DIRECTION, dir.x, dir.y, dir.z);
}

} // namespace dusk
