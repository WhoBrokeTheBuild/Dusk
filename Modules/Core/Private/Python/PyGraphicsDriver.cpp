#include <Python/PyGraphicsDriver.hpp>
#include <Python/PyScriptEvent.hpp>
#include <Dusk/Log.hpp>

namespace Dusk {

typedef struct 
{
    PyObject_HEAD
    GraphicsDriver * Pointer;
    PyObject * WindowResizedEvent;

} PyGraphicsDriver;

static PyMethodDef PyGraphicsDriver_methods[] = {
    { "ProcessEvents",          PyGraphicsDriver_ProcessEvents,         METH_NOARGS,    nullptr },
    { "Render",                 PyGraphicsDriver_Render,                METH_NOARGS,    nullptr },
    { "GetWindowTitle",         PyGraphicsDriver_GetWindowTitle,        METH_NOARGS,    nullptr },
    { "SetWindowTitle",         PyGraphicsDriver_SetWindowTitle,        METH_VARARGS,   nullptr },
    { "GetWindowSize",          PyGraphicsDriver_GetWindowSize,         METH_NOARGS,    nullptr },
    { "SetWindowSize",          PyGraphicsDriver_SetWindowSize,         METH_VARARGS,   nullptr },
    { "EnableDebugShader",      PyGraphicsDriver_EnableDebugShader,     METH_VARARGS,   nullptr },
    { "DisableDebugShader",     PyGraphicsDriver_DisableDebugShader,    METH_NOARGS,    nullptr },
    { nullptr, nullptr, 0, nullptr },
};

static PyMemberDef PyGraphicsDriver_members[] = {
    { "WindowResizedEvent", T_OBJECT, offsetof(PyGraphicsDriver, WindowResizedEvent), READONLY, nullptr },
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
    "Dusk Graphics Driver",                     // tp_doc
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
    0,                                          // tp_del
    0,                                          // tp_version_tag
    0,                                          // tp_finalize
    0,                                          // tp_vectorcall
};

PyObject * New_PyGraphicsDriver(GraphicsDriver * gfx)
{
    PyGraphicsDriver * obj = (PyGraphicsDriver *)PyType_GenericAlloc(&PyGraphicsDriver_type, 1);
    obj->Pointer = gfx;
    obj->WindowResizedEvent = New_PyScriptEvent(dynamic_cast<ScriptEvent *>(&(gfx->WindowResizedEvent)));
    return (PyObject *)obj;
}

PyObject * PyGraphicsDriver_ProcessEvents(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;
    gfx->Pointer->ProcessEvents();
    Py_RETURN_NONE;
}

PyObject * PyGraphicsDriver_Render(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;
    gfx->Pointer->Render();
    Py_RETURN_NONE;
}

PyObject * PyGraphicsDriver_GetWindowTitle(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;
    const auto& title = gfx->Pointer->GetWindowTitle();
    return PyUnicode_FromString(title.c_str());
}

PyObject * PyGraphicsDriver_SetWindowTitle(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;
    const char * title;

    if (!PyArg_ParseTuple(args, "s", &title)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    gfx->Pointer->SetWindowTitle(title);
    Py_RETURN_NONE;
}

PyObject * PyGraphicsDriver_GetWindowSize(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;
    const auto& size = gfx->Pointer->GetWindowSize();
    PyObject * ret = PyTuple_New(2);
    PyTuple_SET_ITEM(ret, 0, PyLong_FromLong(size.x));
    PyTuple_SET_ITEM(ret, 1, PyLong_FromLong(size.y));
    return ret;
}

PyObject * PyGraphicsDriver_SetWindowSize(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;

    ivec2 size;
    if (!PyArg_ParseTuple(args, "(ii)", &size.x, &size.y)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    gfx->Pointer->SetWindowSize(size);
    Py_RETURN_NONE;
}

PyObject * PyGraphicsDriver_EnableDebugShader(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;
    const char * name;

    if (!PyArg_ParseTuple(args, "s", &name)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    gfx->Pointer->EnableDebugShader(name);
    Py_RETURN_NONE;
}

PyObject * PyGraphicsDriver_DisableDebugShader(PyObject * self, PyObject * args)
{
    PyGraphicsDriver * gfx = (PyGraphicsDriver *)self;
    gfx->Pointer->DisableDebugShader();
    Py_RETURN_NONE;
}

// PyObject * PyDusk_GetGraphicsDriver(PyObject * self, PyObject * args)
// {
//     auto gfx = GetGraphicsDriver();
//     if (!gfx) {
//         Py_RETURN_NONE;
//     }

//     return New_PyGraphicsDriver(gfx);
// }

bool PyInit_GraphicsDriver(PyObject * mod) 
{
    if (PyType_Ready(&PyGraphicsDriver_type) == 0) {
        Py_INCREF(&PyGraphicsDriver_type);
        if (PyModule_AddObject(mod, "GraphicsDriver", (PyObject *)&PyGraphicsDriver_type) < 0) {
            Py_DECREF(&PyGraphicsDriver_type);
            Py_DECREF(mod);
            return false;
        }
    }

    return true;
}

} // namespace Dusk