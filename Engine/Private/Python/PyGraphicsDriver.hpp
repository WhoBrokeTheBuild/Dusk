#ifndef DUSK_PY_GRAPHICS_DRIVER_HPP
#define DUSK_PY_GRAPHICS_DRIVER_HPP

#include <Dusk/Graphics/GraphicsDriver.hpp>

#include <Python.h>
#include <structmember.h>

namespace Dusk {

PyObject * PyGraphicsDriver_ProcessEvents(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_SwapBuffers(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_GetWindowTitle(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_SetWindowTitle(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_GetWindowSize(PyObject * self, PyObject * args);

PyObject * PyGraphicsDriver_SetWindowSize(PyObject * self, PyObject * args);

PyObject * PyDusk_GetGraphicsDriver(PyObject * self, PyObject * args);

bool PyInit_GraphicsDriver(PyObject * module);

typedef struct 
{
    PyObject_HEAD
    GraphicsDriver * Pointer;
    PyObject * WindowResizedEvent;

} PyGraphicsDriver;

static PyMethodDef PyGraphicsDriver_methods[] = {
    { "ProcessEvents",          PyGraphicsDriver_ProcessEvents,         METH_NOARGS,    nullptr },
    { "SwapBuffers",            PyGraphicsDriver_SwapBuffers,           METH_NOARGS,    nullptr },
    { "GetWindowTitle",         PyGraphicsDriver_GetWindowTitle,        METH_NOARGS,    nullptr },
    { "SetWindowTitle",         PyGraphicsDriver_SetWindowTitle,        METH_VARARGS,   nullptr },
    { "GetWindowSize",          PyGraphicsDriver_GetWindowSize,         METH_NOARGS,    nullptr },
    { "SetWindowSize",          PyGraphicsDriver_SetWindowSize,         METH_VARARGS,   nullptr },
    { nullptr, nullptr, 0, nullptr },
};

static PyMemberDef PyGraphicsDriver_members[] = {
    { "WindowResizedEvent", T_OBJECT_EX, offsetof(PyGraphicsDriver, WindowResizedEvent), READONLY, nullptr },
    { nullptr, 0, 0, 0, nullptr },
};

static PyTypeObject PyGraphicsDriver_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Dusk.GraphicsDriver",                      // tp_name 
    sizeof(PyGraphicsDriver),                   // tp_basicsize
    0,                                          // tp_itemsize
    0,                                          // tp_dealloc
    0,                                          // tp_print
    0,                                          // tp_getattr
    0,                                          // tp_setattr
    0,                                          // tp_compare
    0,                                          // tp_repr
    0,                                          // tp_as_number
    0,                                          // tp_as_sequence
    0,                                          // tp_as_mapping
    0,                                          // tp_hash
    0,                                          // tp_call
    0,                                          // tp_str
    0,                                          // tp_getattro
    0,                                          // tp_setattro
    0,                                          // tp_as_buffer
    Py_TPFLAGS_DEFAULT,                         // tp_flags
    0,                                          // tp_doc
    0,                                          // tp_traverse
    0,                                          // tp_clear
    0,                                          // tp_richcompare
    0,                                          // tp_weaklistoffset
    0,                                          // tp_iter
    0,                                          // tp_iternext
    PyGraphicsDriver_methods,                   // tp_methods
    PyGraphicsDriver_members,                   // tp_members
    0,                                          // tp_getset
    0,                                          // tp_base
    0,                                          // tp_dict
    0,                                          // tp_descr_get
    0,                                          // tp_descr_set
    0,                                          // tp_dictoffset
    0,                                          // tp_init
    0,                                          // tp_alloc
    0,                                          // tp_new
    0,                                          // tp_free
    0,                                          // tp_is_gc
    0,                                          // tp_bases
    0,                                          // tp_mro
    0,                                          // tp_cache
    0,                                          // tp_subclasses
    0,                                          // tp_weaklist
#if PYTHON_API_VERSION >= 1012
    0                                           // tp_del
#endif
};

} // namespace Dusk

#endif // DUSK_PY_GRAPHICS_DRIVER_HPP