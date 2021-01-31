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
#include <Dusk/Buffer.hpp>
#include <Dusk/Scene.hpp>

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

    virtual bool Initialize();

    virtual void Terminate() = 0;

    virtual void SetWindowTitle(const std::string& title) = 0;

    virtual std::string GetWindowTitle() = 0;

    virtual void SetWindowSize(const ivec2& size) = 0;

    virtual ivec2 GetWindowSize() = 0;

    virtual void SetBackbufferCount(unsigned backbufferCount) {
        _backbufferCount = backbufferCount;
    }

    virtual unsigned GetBackbufferCount() const {
        return _backbufferCount;
    }

    virtual void SetClearColor(const vec4& color) {
        _clearColor = color;
    }

    virtual vec4 GetClearColor() const {
        return _clearColor;
    }

    virtual void ProcessEvents() = 0;
    
    virtual void Render() = 0;

    virtual std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader) = 0;

    virtual std::shared_ptr<Texture> CreateTexture() = 0;

    virtual std::shared_ptr<Shader> CreateShader() = 0;

    virtual std::shared_ptr<Mesh> CreateMesh() = 0;

    virtual std::unique_ptr<Primitive> CreatePrimitive() = 0;

    virtual UpdateContext * GetUpdateContext();

    virtual RenderContext * GetRenderContext();

    Event<Dusk::WindowResizedEventData> WindowResizedEvent;

    inline void SetCurrentScene(Scene * currentScene) {
        _currentScene = currentScene;
    }

protected:

    // Push Constants?

    vec4 _clearColor = vec4(0.392f, 0.584f, 0.929f, 1.0f);

    std::unique_ptr<UpdateContext> _updateContext;

    std::unique_ptr<RenderContext> _renderContext;

    unsigned _backbufferCount = 2;

    Scene * _currentScene = nullptr;

}; // class GraphicsDriver

DUSK_ENGINE_API void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver);

DUSK_ENGINE_API GraphicsDriver * GetGraphicsDriver();

} // namespace Dusk

#endif // DUSK_GRAPHICS_DRIVER_HPP