#include <Python/PyGraphicsDriver.hpp>
#include <Python/PyScriptEvent.hpp>
#include <Dusk/Log.hpp>

namespace Dusk {

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
        return nullptr;
    }

    gfx->Pointer->SetWindowSize(size);
    Py_RETURN_NONE;
}

PyObject * PyDusk_GetGraphicsDriver(PyObject * self, PyObject * args)
{
    auto gfx = GetGraphicsDriver();
    if (!gfx) {
        PyErr_SetString(PyExc_RuntimeError, "No Graphics Driver found.");
        Py_RETURN_NONE;
    }

    PyScriptEvent * windowResizedEvt = (PyScriptEvent *)PyType_GenericAlloc(&PyScriptEvent_type, 1);
    windowResizedEvt->Pointer = &(gfx->WindowResizedEvent);

    PyGraphicsDriver * obj = (PyGraphicsDriver *)PyType_GenericAlloc(&PyGraphicsDriver_type, 1);
    obj->Pointer = gfx;
    obj->WindowResizedEvent = (PyObject *)windowResizedEvt;
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