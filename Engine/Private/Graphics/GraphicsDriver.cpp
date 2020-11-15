#include <Dusk/Graphics/GraphicsDriver.hpp>

namespace Dusk {

static std::unique_ptr<GraphicsDriver> _GraphicsDriver(nullptr);

PyObject * WindowResizedEventData::GetPyObject() const
{
    PyObject * dict = PyDict_New();

    PyObject * size = PyTuple_New(2);
    PyTuple_SET_ITEM(size, 0, PyLong_FromLong(Size.x));
    PyTuple_SET_ITEM(size, 1, PyLong_FromLong(Size.y));
    PyDict_SetItemString(dict, "Size", size);

    PyObject * delta = PyTuple_New(2);
    PyTuple_SET_ITEM(delta, 0, PyLong_FromLong(Delta.x));
    PyTuple_SET_ITEM(delta, 1, PyLong_FromLong(Delta.y));
    PyDict_SetItemString(dict, "Delta", delta);

    return dict;
}

DUSK_CORE_API 
void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver)
{
    _GraphicsDriver = std::move(driver);
}

DUSK_CORE_API 
GraphicsDriver * GetGraphicsDriver()
{
    return _GraphicsDriver.get();
}

DUSK_CORE_API
UpdateContext * GraphicsDriver::GetUpdateContext()
{
    return _updateContext.get();
}

DUSK_CORE_API
RenderContext * GraphicsDriver::GetRenderContext()
{
    return _renderContext.get();
}

} // namespace Dusk