#ifndef DUSK_SYSTEM_HPP
#define DUSK_SYSTEM_HPP

#include <Dusk/Config.hpp>
#include <Dusk/UpdateContext.hpp>
#include <Dusk/RenderContext.hpp>

namespace Dusk {

class System
{
public:

    DISALLOW_COPY_AND_ASSIGN(System)

    explicit System() = default;

    virtual ~System() = default;

    virtual inline void Update(UpdateContext * ctx) { }

    virtual inline void Render(RenderContext * ctx) { }

}; // class System

} // namespace Dusk

#endif // DUSK_SYSTEM_HPP