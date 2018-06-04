#include "dusk/scene/IComponent.hpp"

#include <dusk/scene/Actor.hpp>

namespace dusk {

std::unordered_map<std::string, IComponent::CompTypeFunc> IComponent::_TypeFuncs;

void IComponent::RegisterType(const std::string& type, CompTypeFunc func)
{
    if (_TypeFuncs.find(type) == _TypeFuncs.end()) {
        _TypeFuncs[type] = func;
    }
}

IComponent * IComponent::CreateInstanceOfType(const std::string& type, Actor * actor)
{
    if (_TypeFuncs.find(type) == _TypeFuncs.end()) {
        DuskLogWarn("Failed to create IComponent of type '%s'", type.c_str());
        return nullptr;
    }
    return _TypeFuncs[type](actor);
}

IComponent::IComponent(Actor * actor)
        : _actor(actor)
{
}

} // namespace dusk
