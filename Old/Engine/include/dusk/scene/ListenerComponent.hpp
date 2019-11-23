#ifndef DUSK_LISTENER_COMPONENT
#define DUSK_LISTENER_COMPONENT

#include <dusk/core/Macros.hpp>
#include <dusk/scene/ActorComponent.hpp>

#include <memory>

namespace dusk {

class ListenerComponent : public ActorComponent
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(ListenerComponent)

    ListenerComponent() = default;

    virtual ~ListenerComponent() = default;

    /// Methods

    void Update(UpdateContext&) override;

}; // class ListenerComponent

} // namespace dusk

#endif // DUSK_LISTENER_COMPONENT
