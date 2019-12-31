#ifndef DUSK_COMPONENT_HPP
#define DUSK_COMPONENT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/UpdateContext.hpp>
#include <Dusk/RenderContext.hpp>

namespace Dusk {

class Entity;

class DUSK_CORE_API IComponent 
{
public:

    DISALLOW_COPY_AND_ASSIGN(IComponent)

    explicit IComponent() = default;

    virtual ~IComponent() = default;

    virtual void Attach(Entity * entity) = 0;

    virtual void Detach() = 0;

    virtual inline bool IsAttached() const {
        return (_entity != nullptr);
    }

    virtual inline void Update(UpdateContext * ctx) { }

    virtual inline void Render(RenderContext * ctx) { }

private:

    Entity * _entity = nullptr;

}; // class IComponent

} // namespace Dusk

#endif // DUSK_COMPONENT_HPP