#ifndef DUSK_LISTENER_COMPONENT
#define DUSK_LISTENER_COMPONENT

#include <dusk/Config.hpp>
#include <dusk/asset/Sound.hpp>
#include <dusk/core/OpenAL.hpp>
#include <dusk/scene/ActorComponent.hpp>

#include <memory>

namespace dusk {

class ListenerComponent : public ActorComponent
{
public:

    ListenerComponent() = default;

    virtual ~ListenerComponent() = default;

    virtual void Update(UpdateContext& ctx) override;

}; // class ListenerComponent

} // namespace dusk

#endif // DUSK_LISTENER_COMPONENT
