#include <Python/PyScriptEvent.hpp>
#include <Dusk/Log.hpp>

#include <Dusk/GraphicsDriver.hpp>

namespace Dusk {

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
    0,                                          // tp_vectorcall_offset
    0,                                          // tp_getattr
    0,                                          // tp_setattr
    0,                                          // tp_as_async
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
    "Dusk Script Event",                        // tp_doc
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
    0,                                          // tp_del
    0,                                          // tp_version_tag
    0,                                          // tp_finalize
    0,                                          // tp_vectorcall
};

PyObject * New_PyScriptEvent(ScriptEvent * evt)
{
    PyScriptEvent * obj = (PyScriptEvent *)PyType_GenericAlloc(&PyScriptEvent_type, 1);
    obj->Pointer = evt;
    return (PyObject *)obj;
}

PyObject * PyScriptEvent_AddListener(PyObject * self, PyObject * args)
{
    PyScriptEvent * event = (PyScriptEvent *)self;

    PyObject * listener;
    if (PyArg_ParseTuple(args, "O", &listener)) {
        unsigned id = event->Pointer->AddPythonListener(listener);
        return PyLong_FromLong(id);
    }

    Py_RETURN_NONE;
}

PyObject * PyScriptEvent_RemoveAllListeners(PyObject * self, PyObject * args)
{
    PyScriptEvent * event = (PyScriptEvent *)self;

    event->Pointer->RemoveAllPythonListeners();

    Py_RETURN_NONE;
}

// PyObject * PyDusk_Call(PyObject * self, PyObject * args);

bool PyInit_ScriptEvent(PyObject * mod)
{
    if (PyType_Ready(&PyScriptEvent_type) == 0) {
        Py_INCREF(&PyScriptEvent_type);
        if (PyModule_AddObject(mod, "ScriptEvent", (PyObject *)&PyScriptEvent_type) < 0) {
            Py_DECREF(&PyScriptEvent_type);
            Py_DECREF(mod);
            return false;
        }
    }

    return true;
}

} // namespace Dusk
