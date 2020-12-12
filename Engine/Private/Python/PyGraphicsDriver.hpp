#ifndef DUSK_PY_GRAPHICS_DRIVER_HPP
#define DUSK_PY_GRAPHICS_DRIVER_HPP

#include <Dusk/GraphicsDriver.hpp>

#include <Python.h>
#include <structmember.h>

namespace Dusk {

PyObject * New_PyGraphicsDriver(GraphicsDriver * gfx);

PyObject * PyGraphicsDriver_ProcessEvents(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_SwapBuffers(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_GetWindowTitle(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_SetWindowTitle(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_GetWindowSize(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_SetWindowSize(PyObject * self, PyObject * args);

PyObject * PyDusk_GetGraphicsDriver(PyObject * self, PyObject * args);

bool PyInit_GraphicsDriver(PyObject * mod);

} // namespace Dusk

#endif // DUSK_PY_GRAPHICS_DRIVER_HPP