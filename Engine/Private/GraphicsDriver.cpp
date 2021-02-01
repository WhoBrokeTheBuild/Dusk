#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/InputDriver.hpp>

namespace Dusk {

static std::unique_ptr<GraphicsDriver> _GraphicsDriver(nullptr);

DUSK_ENGINE_API
void GraphicsDriver::InitializeUpdateContext()
{
    if (!_updateContext) {
        _updateContext.reset(New UpdateContext());
    }

    _updateContext->SetTargetFPS(60.0f);
    _updateContext->ResetTime();
}

DUSK_ENGINE_API
void GraphicsDriver::InitializeRenderContext()
{
    if (!_renderContext) {
        _renderContext.reset(New RenderContext());
    }

    auto globals = _renderContext->GetShaderGlobals();
    globals->Resolution = GetWindowSize();
    globals->Mouse = { 0, 0 };
    globals->FrameCount = 0;
    globals->FrameSpeedRatio = _updateContext->GetFrameSpeedRatio();
    // globals->TotalTime = _updateContext->GetTotalDuration();

    auto transform = _renderContext->GetShaderTransform();
    transform->Model = mat4(1.0f);
    transform->View = mat4(1.0f);
    transform->Projection = mat4(1.0f);
    transform->MVP = mat4(1.0f);
}

DUSK_ENGINE_API
void GraphicsDriver::Render()
{
    UpdateContext * updateCtx = GetUpdateContext();

    // Update FPS
    _fpsUpdateElapsedTime += updateCtx->GetPreviousFrameDuration();
    if (_fpsUpdateFrameCount >= updateCtx->GetTargetFPS()) {
        float currentFPS = (float)_fpsUpdateFrameCount / (float)_fpsUpdateElapsedTime.count();
        currentFPS *= 1000000.0f; // Microseconds -> Seconds

        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.2f FPS", currentFPS);
        UpdateWindowTitle(_windowTitle + " - " + buffer); // FIX

        _fpsUpdateElapsedTime = 0ms;
        _fpsUpdateFrameCount = 0;
    }

    ++_fpsUpdateFrameCount;
    
    auto input = GetInputDriver();

    auto globals = _renderContext->GetShaderGlobals();
    globals->Resolution = GetWindowSize();
    globals->Mouse = input->GetMouseCoordinates();
    ++globals->FrameCount;
    globals->FrameSpeedRatio = _updateContext->GetFrameSpeedRatio();
    // globals->TotalTime = _updateContext->GetTotalDuration();
}

DUSK_ENGINE_API
UpdateContext * GraphicsDriver::GetUpdateContext()
{
    return _updateContext.get();
}

DUSK_ENGINE_API
RenderContext * GraphicsDriver::GetRenderContext()
{
    return _renderContext.get();
}

PyObject * WindowResizedEventData::GetPyObject() const
{
    PyObject * dict = PyDict_New();

    PyObject * size = PyTuple_New(2);
    PyTuple_SET_ITEM(size, 0, PyLong_FromLong(Size.x));
    PyTuple_SET_ITEM(size, 1, PyLong_FromLong(Size.y));
    PyDict_SetItemString(dict, "Size", size);

    return dict;
}

DUSK_ENGINE_API 
void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver)
{
    if (_GraphicsDriver) {
        _GraphicsDriver->Terminate();
    }
    _GraphicsDriver = std::move(driver);
}

DUSK_ENGINE_API 
GraphicsDriver * GetGraphicsDriver()
{
    return _GraphicsDriver.get();
}

} // namespace Dusk