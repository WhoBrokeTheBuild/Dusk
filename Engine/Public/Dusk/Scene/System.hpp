#ifndef DUSK_SYSTEM_HPP
#define DUSK_SYSTEM_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/UpdateContext.hpp>
#include <Dusk/RenderContext.hpp>

namespace Dusk {

class ISystem
{
public:

    DISALLOW_COPY_AND_ASSIGN(ISystem)

    explicit ISystem() = default;

    virtual ~ISystem() = default;

    virtual inline void Update(UpdateContext * ctx) { }

    virtual inline void Render(RenderContext * ctx) { }

}; // class ISystem

} // namespace Dusk

#endif // DUSK_SYSTEM_HPP