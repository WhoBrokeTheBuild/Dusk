#ifndef DUSK_ICOMPONENT_HPP
#define DUSK_ICOMPONENT_HPP

#include <dusk/core/Context.hpp>

#include <memory>
#include <string>

namespace dusk {

class Actor;

class ActorComponent
{
public:

    ActorComponent() = default;

    virtual ~ActorComponent() = default;

    void SetActor(Actor * actor);

    Actor * GetActor() const { 
        return _actor;
    }

    virtual void HandleEvent(SDL_Event * evt) { }
    
    virtual void SceneStart() { }

    virtual void SceneStop() { }

    virtual void Update(UpdateContext& ctx) { }
    
    virtual void Render(RenderContext& ctx) { }

    virtual void Print(std::string indent);

private:

    Actor * _actor = nullptr;

}; // class IComponent

} // namespace dusk

#endif // DUSK_ICOMPONENT_HPP
