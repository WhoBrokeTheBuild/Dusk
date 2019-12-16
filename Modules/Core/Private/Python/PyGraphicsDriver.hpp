#ifndef DUSK_PY_GRAPHICS_DRIVER_HPP
#define DUSK_PY_GRAPHICS_DRIVER_HPP

#include <Dusk/Graphics/GraphicsDriver.hpp>

#include <Python.h>

namespace Dusk {

typedef struct 
{
    PyObject_HEAD
    IGraphicsDriver * Pointer;

} PyGraphicsDriver;

static PyMethodDef PyGraphicsDriver_methods[] = {
    { nullptr, nullptr, 0, nullptr },
};

static PyTypeObject PyGraphicsDriver_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Dusk.Thing",                               // tp_name
    sizeof(IGraphicsDriver),                    // tp_basicsize
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
    Py_TPFLAGS_DEFAULT                      ,   // tp_flags
    0,                                          // tp_doc
    0,                                          // tp_traverse
    0,                                          // tp_clear
    0,                                          // tp_richcompare
    0,                                          // tp_weaklistoffset
    0,                                          // tp_iter
    0,                                          // tp_iternext
    PyGraphicsDriver_methods,                   // tp_methods
    0,                                          // tp_members
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

PyObject * PyDusk_GetGraphicsDriver(PyObject * self, PyObject * args);

} // namespace Dusk

#endif // DUSK_PY_GRAPHICS_DRIVER_HPP