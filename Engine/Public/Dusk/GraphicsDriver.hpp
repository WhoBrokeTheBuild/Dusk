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
#include <Dusk/Time.hpp>
#include <Dusk/String.hpp>

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

    virtual bool Initialize() = 0;

    virtual void Terminate() = 0;

    virtual void InitializeUpdateContext();

    virtual void InitializeRenderContext();

    virtual bool InitializeConstantBuffers();

    virtual bool InitializeDefaults();

    virtual void SetWindowTitle(const string& title) {
        _windowTitle = title;
        UpdateWindowTitle(title);
    }

    virtual string GetWindowTitle() {
        return _windowTitle;
    }

    virtual void SetWindowSize(const ivec2& size) {
        _windowSize = size;
        UpdateWindowSize(size);
    }

    virtual ivec2 GetWindowSize() {
        return _windowSize;
    }

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
    
    virtual void Render();

    virtual std::shared_ptr<Buffer> CreateBuffer() = 0;

    virtual std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader) = 0;

    virtual std::shared_ptr<Texture> CreateTexture() = 0;

    virtual std::shared_ptr<Shader> CreateShader() = 0;

    virtual std::shared_ptr<Mesh> CreateMesh() = 0;

    virtual std::shared_ptr<Material> CreateMaterial() = 0;

    virtual std::shared_ptr<Primitive> CreatePrimitive() = 0;

    virtual UpdateContext * GetUpdateContext();

    virtual RenderContext * GetRenderContext();

    Event<Dusk::WindowResizedEventData> WindowResizedEvent;

    virtual inline std::shared_ptr<Buffer> GetShaderGlobalsBuffer() {
        return _shaderGlobalsBuffer;
    }

    virtual inline std::shared_ptr<Texture> GetDefaultTexture() {
        return _defaultTexture;
    }

    virtual inline std::shared_ptr<Pipeline> GetDefaultPipeline() {
        return _defaultPipeline;
    }

    virtual inline std::shared_ptr<Material> GetDefaultMaterial() {
        return _defaultMaterial;
    }

    virtual void EnableDebugShader(const std::string& name);

    virtual void DisableDebugShader();

    virtual Shader * GetActiveDebugShader();

protected:

    virtual void UpdateWindowTitle(const string& title) = 0;

    virtual void UpdateWindowSize(const ivec2& size) = 0;

    string _windowTitle = "Dusk";

    ivec2 _windowSize = { 640, 480 };

    microseconds _fpsUpdateElapsedTime = 0us;

    uintmax_t _fpsUpdateFrameCount = 0;

    // Push Constants?

    vec4 _clearColor = vec4(0.392f, 0.584f, 0.929f, 1.0f);

    std::unique_ptr<UpdateContext> _updateContext;

    std::unique_ptr<RenderContext> _renderContext;

    unsigned _backbufferCount = 2;

    std::shared_ptr<Buffer> _shaderGlobalsBuffer;

    std::shared_ptr<Texture> _defaultTexture;

    std::shared_ptr<Pipeline> _defaultPipeline;

    std::shared_ptr<Material> _defaultMaterial;

    std::unordered_map<std::string, std::shared_ptr<Dusk::Shader>> _debugShaderMap;

    std::shared_ptr<Shader> _activeDebugShader;

}; // class GraphicsDriver

DUSK_ENGINE_API
void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver);

DUSK_ENGINE_API
GraphicsDriver * GetGraphicsDriver();

} // namespace Dusk

#endif // DUSK_GRAPHICS_DRIVER_HPP