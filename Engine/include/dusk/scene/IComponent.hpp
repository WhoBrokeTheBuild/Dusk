#ifndef DUSK_ICOMPONENT_HPP
#define DUSK_ICOMPONENT_HPP

namespace dusk {

class Actor;

class IComponent
{
public:

    IComponent(Actor * actor);

    virtual ~IComponent() = default;

    Actor * GetActor() { return _actor; }

private:

    Actor * _actor;

}; // class IComponent

} // namespace dusk

#endif // DUSK_ICOMPONENT_HPP
