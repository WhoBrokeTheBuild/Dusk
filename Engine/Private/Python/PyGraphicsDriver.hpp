#ifndef DUSK_PY_GRAPHICS_DRIVER_HPP
#define DUSK_PY_GRAPHICS_DRIVER_HPP

#include <Dusk/Graphics/GraphicsDriver.hpp>

#include <Python.h>

namespace Dusk {

PyObject * PyGraphicsDriver_ProcessEvents(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_SwapBuffers(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_SetWindowSize(PyObject * self, PyObject * args);

PyObject * PyDusk_GetGraphicsDriver(PyObject * self, PyObject * args);

bool PyInit_GraphicsDriver(PyObject * module);

} // namespace Dusk

#endif // DUSK_PY_GRAPHICS_DRIVER_HPP