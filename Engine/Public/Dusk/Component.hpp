#ifndef DUSK_COMPONENT_HPP
#define DUSK_COMPONENT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/UpdateContext.hpp>
#include <Dusk/RenderContext.hpp>

namespace Dusk {

class Entity;

class DUSK_ENGINE_API Component : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Component)

    Component() = default;

    virtual ~Component() = default;

    virtual inline void Attach(Entity * entity) {
        _entity = entity;
    }

    virtual inline void Detach() {
        _entity = nullptr;
    }

    virtual inline bool IsAttached() const {
        return (_entity != nullptr);
    }

    virtual Entity * GetEntity() const {
        return _entity;
    }

    virtual inline void Update(UpdateContext * ctx) { }

    virtual inline void Render(RenderContext * ctx) { }

private:

    Entity * _entity = nullptr;

}; // class Component

} // namespace Dusk

#endif // DUSK_COMPONENT_HPP