#ifndef DUSK_GRAPHICS_DRIVER_HPP
#define DUSK_GRAPHICS_DRIVER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Graphics/Texture.hpp>
#include <Dusk/Graphics/Shader.hpp>

#include <string>
#include <vector>
#include <memory>

namespace Dusk {

class DUSK_CORE_API IGraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(IGraphicsDriver)

    explicit IGraphicsDriver() = default;

    virtual ~IGraphicsDriver() = default;

    virtual void SetWindowTitle(const std::string& title) = 0;

    virtual std::string GetWindowTitle() = 0;

    virtual void SetWindowSize(const ivec2& size) = 0;

    virtual ivec2 GetWindowSize() = 0;

    virtual void ProcessEvents() = 0;
    
    virtual void SwapBuffers() = 0;

    virtual ITexture * CreateTexture() = 0;

    virtual IShader * CreateShader() = 0;

}; // class IGraphicsDriver

DUSK_CORE_API void SetGraphicsDriver(std::unique_ptr<IGraphicsDriver> && driver);

DUSK_CORE_API IGraphicsDriver * GetGraphicsDriver();

} // namespace Dusk

#endif // DUSK_GRAPHICS_DRIVER_HPP