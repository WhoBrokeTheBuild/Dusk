#ifndef DUSK_ICOMPONENT_HPP
#define DUSK_ICOMPONENT_HPP

#include <dusk/core/Context.hpp>

#include <memory>

namespace dusk {

class Actor;

class ActorComponent;
typedef std::unique_ptr<ActorComponent> ActorComponentPtr;

class ActorComponent
{
public:

    ActorComponent() = default;

    ActorComponent(Actor * actor);

    virtual ~ActorComponent() = default;

    void SetActor(Actor * actor);

    Actor * GetActor() const { 
        return _actor;
    }

    virtual void HandleEvent(SDL_Event * evt) { }

    virtual void Update(UpdateContext& ctx) { }
    
    virtual void Render(RenderContext& ctx) { }

    virtual void Print(std::string indent);

private:

    Actor * _actor = nullptr;

}; // class IComponent

} // namespace dusk

#endif // DUSK_ICOMPONENT_HPP
