#include "dusk/scene/ActorComponent.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/scene/Actor.hpp>

namespace dusk {

void ActorComponent::SetActor(Actor * actor)
{
    _actor = actor;
}

void ActorComponent::Print(std::string indent) 
{
    DuskLog("%s _actor = %p", indent, _actor);
}

} // namespace dusk
