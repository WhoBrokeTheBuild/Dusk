#ifndef DUSK_GRAPHICS_DRIVER_HPP
#define DUSK_GRAPHICS_DRIVER_HPP

#include <Dusk/Core.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>

#include <string>

namespace Dusk {

class DUSK_CORE_API IGraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(IGraphicsDriver)

    explicit IGraphicsDriver() = default;

    virtual ~IGraphicsDriver() = default;

    virtual void SetTitle(const std::string& title) = 0;

    virtual std::string GetTitle() = 0;

    virtual void SetSize(const ivec2& size) = 0;

    virtual ivec2 GetSize() = 0;

    virtual void ProcessEvents() = 0;
    
    virtual void SwapBuffers() = 0;

}; // class IGraphicsDriver

DUSK_CORE_API void SetGraphicsDriver(IGraphicsDriver * driver);

DUSK_CORE_API IGraphicsDriver * GetGraphicsDriver();

} // namespace Dusk

#endif // DUSK_GRAPHICS_DRIVER_HPP