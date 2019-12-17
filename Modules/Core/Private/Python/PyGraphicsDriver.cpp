#include <Python/PyGraphicsDriver.hpp>
#include <Dusk/Log.hpp>

namespace Dusk {

typedef struct 
{
    PyObject_HEAD
    IGraphicsDriver * Pointer;

} PyGraphicsDriver;

PyObject * PyGraphicsDriver_ProcessEvents(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;
    gfx->Pointer->ProcessEvents();
    Py_RETURN_NONE;
}

PyObject * PyGraphicsDriver_SwapBuffers(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;
    gfx->Pointer->SwapBuffers();
    Py_RETURN_NONE;
}

PyObject * PyGraphicsDriver_SetWindowSize(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;

    ivec2 size;
    if (!PyArg_ParseTuple(args, "(ii)", &size.x, &size.y)) {
        PyErr_BadArgument();
        return nullptr;
    }

    gfx->Pointer->SetWindowSize(size);
    Py_RETURN_NONE;
}

static PyMethodDef PyGraphicsDriver_methods[] = {
    { "ProcessEvents",      PyGraphicsDriver_ProcessEvents,     METH_NOARGS,    nullptr },
    { "SwapBuffers",        PyGraphicsDriver_SwapBuffers,       METH_NOARGS,    nullptr },
    { "SetWindowSize",      PyGraphicsDriver_SetWindowSize,     METH_VARARGS,   nullptr },
    { nullptr, nullptr, 0, nullptr },
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

PyObject * PyDusk_GetGraphicsDriver(PyObject * self, PyObject * args)
{
    auto gfx = GetGraphicsDriver();
    if (!gfx) {
        PyErr_SetString(PyExc_RuntimeError, "No Graphics Driver found.");
        Py_RETURN_NONE;
    }

    PyGraphicsDriver * obj = (PyGraphicsDriver *)PyType_GenericAlloc(&PyGraphicsDriver_type, 1);
    obj->Pointer = gfx;
    Py_INCREF(obj);
    return (PyObject *)obj;
}

bool PyInit_GraphicsDriver(PyObject * module) 
{
    if (PyType_Ready(&PyGraphicsDriver_type) == 0) {
        Py_INCREF(&PyGraphicsDriver_type);
        if (PyModule_AddObject(module, "GraphicsDriver", (PyObject *)&PyGraphicsDriver_type) < 0) {
            Py_DECREF(&PyGraphicsDriver_type);
            Py_DECREF(module);
            return false;
        }
    }

    return true;
}

} // namespace Dusk