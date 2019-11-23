#ifndef DUSK_ICOMPONENT_HPP
#define DUSK_ICOMPONENT_HPP

#include <dusk/core/Context.hpp>

#include <string>

namespace dusk {

class Actor;

class ActorComponent
{
public:

    /// Boilerplate

    ActorComponent() = default;

    virtual ~ActorComponent() = default;

    /// Methods

    void SetActor(Actor *);

    Actor * GetActor() const { 
        return _actor;
    }

    virtual void HandleEvent(SDL_Event *) { }
    
    virtual void SceneStart() { }

    virtual void SceneStop() { }

    virtual void Update(UpdateContext&) { }
    
    virtual void Render(RenderContext&) { }

    virtual void Print(std::string indent);

private:

    /// Variables

    Actor * _actor = nullptr;

}; // class IComponent

} // namespace dusk

#endif // DUSK_ICOMPONENT_HPP
