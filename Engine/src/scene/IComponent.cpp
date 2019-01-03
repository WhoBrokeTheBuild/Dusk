#include "dusk/scene/IComponent.hpp"

#include <dusk/scene/Actor.hpp>

namespace dusk {

IComponent::IComponent(Actor * actor)
        : _actor(actor)
{ }

} // namespace dusk
