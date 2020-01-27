#include <Python/PyScriptEvent.hpp>
#include <Dusk/Log.hpp>
namespace Dusk {

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

bool PyInit_ScriptEvent(PyObject * module)
{
    if (PyType_Ready(&PyScriptEvent_type) == 0) {
        Py_INCREF(&PyScriptEvent_type);
        if (PyModule_AddObject(module, "ScriptEvent", (PyObject *)&PyScriptEvent_type) < 0) {
            Py_DECREF(&PyScriptEvent_type);
            Py_DECREF(module);
            return false;
        }
    }

    return true;
}

} // namespace Dusk
