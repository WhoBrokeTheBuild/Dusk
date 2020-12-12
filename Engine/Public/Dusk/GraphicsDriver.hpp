#ifndef DUSK_GRAPHICS_DRIVER_HPP
#define DUSK_GRAPHICS_DRIVER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/UpdateContext.hpp>
#include <Dusk/RenderContext.hpp>
#include <Dusk/Pipeline.hpp>
#include <Dusk/Texture.hpp>
#include <Dusk/Shader.hpp>
#include <Dusk/Mesh.hpp>
#include <Dusk/Event.hpp>

#include <string>
#include <vector>
#include <memory>

namespace Dusk {

class DUSK_ENGINE_API WindowResizedEventData : public EventData
{
public:

    ivec2 Size;

    PyObject * GetPyObject() const override;

}; // class WindowResizedEventData

class DUSK_ENGINE_API GraphicsDriver : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(GraphicsDriver)

    GraphicsDriver() = default;

    virtual ~GraphicsDriver() = default;

    inline std::string GetClassID() const override {
        return "Dusk::GraphicsDriver";
    }

    virtual bool Initialize() = 0;

    virtual void Terminate() = 0;

    virtual void SetWindowTitle(const std::string& title) = 0;

    virtual std::string GetWindowTitle() = 0;

    virtual void SetWindowSize(const ivec2& size) = 0;

    virtual ivec2 GetWindowSize() = 0;

    virtual void ProcessEvents() = 0;
    
    virtual void SwapBuffers() = 0;

    virtual std::shared_ptr<Pipeline> CreatePipeline() = 0;

    virtual std::shared_ptr<Texture> CreateTexture() = 0;

    virtual std::shared_ptr<Shader> CreateShader() = 0;

    virtual std::shared_ptr<Mesh> CreateMesh() = 0;

    virtual UpdateContext * GetUpdateContext();

    virtual RenderContext * GetRenderContext();

    virtual bool SetShaderData(const std::string& name, size_t size, void * data) = 0;

    Event<Dusk::WindowResizedEventData> WindowResizedEvent;

private:

    std::unique_ptr<UpdateContext> _updateContext = std::make_unique<UpdateContext>();

    std::unique_ptr<RenderContext> _renderContext = std::make_unique<RenderContext>();

}; // class GraphicsDriver

DUSK_ENGINE_API void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver);

DUSK_ENGINE_API GraphicsDriver * GetGraphicsDriver();

} // namespace Dusk

#endif // DUSK_GRAPHICS_DRIVER_HPP