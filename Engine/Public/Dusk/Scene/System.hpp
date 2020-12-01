#ifndef DUSK_SYSTEM_HPP
#define DUSK_SYSTEM_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/UpdateContext.hpp>
#include <Dusk/RenderContext.hpp>

namespace Dusk {

class DUSK_ENGINE_API System : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(System)

    System() = default;

    virtual ~System() = default;

    inline std::string GetClassID() const {
        return "Dusk::Entity";
    }

    virtual inline void Update(UpdateContext * ctx) { }

    virtual inline void Render(RenderContext * ctx) { }

}; // class System

} // namespace Dusk

#endif // DUSK_SYSTEM_HPP