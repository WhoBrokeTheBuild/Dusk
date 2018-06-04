#ifndef DUSK_ICOMPONENT_HPP
#define DUSK_ICOMPONENT_HPP

#include <dusk/core/Event.hpp>
#include <functional>

namespace dusk {

class Actor;

class IComponent : public ICallbackHost
{
public:

    typedef std::function<IComponent*(Actor * actor)> CompTypeFunc;

    static void RegisterType(const std::string& type, CompTypeFunc func);

    static IComponent * CreateInstanceOfType(const std::string& type, Actor * actor);

    IComponent(Actor * actor);
    virtual ~IComponent() = default;

    virtual void Serialize(nlohmann::json& data) = 0;
    virtual void Deserialize(nlohmann::json& data) = 0;

    Actor * GetActor() { return _actor; }

private:

    Actor * _actor;

    static std::unordered_map<std::string, CompTypeFunc> _TypeFuncs;

}; // class IComponent

} // namespace dusk

#endif // DUSK_ICOMPONENT_HPP
