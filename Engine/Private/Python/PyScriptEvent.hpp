#ifndef DUSK_PY_SCRIPT_EVENT_HPP
#define DUSK_PY_SCRIPT_EVENT_HPP

#include <Dusk/Event.hpp>

#include <Python.h>

namespace Dusk {

PyObject * PyScriptEvent_AddListener(PyObject * self, PyObject * args);

PyObject * PyScriptEvent_RemoveAllListeners(PyObject * self, PyObject * args);

// PyObject * PyDusk_Call(PyObject * self, PyObject * args);

bool PyInit_ScriptEvent(PyObject * module);

typedef struct
{
    PyObject_HEAD
    ScriptEvent * Pointer;
    
} PyScriptEvent;

static PyMethodDef PyScriptEvent_methods[] = {
    { "AddListener",            PyScriptEvent_AddListener,          METH_VARARGS,    nullptr },
    { "RemoveAllListeners",     PyScriptEvent_RemoveAllListeners,   METH_VARARGS,    nullptr },
    { nullptr, nullptr, 0, nullptr },
};

static PyTypeObject PyScriptEvent_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Dusk.ScriptEvent",                         // tp_name 
    sizeof(PyScriptEvent),                      // tp_basicsize
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
    PyScriptEvent_methods,                      // tp_methods
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

} // namespace Dusk

#endif // DUSK_PY_SCRIPT_EVENT_HPP