#include <Python/PyDusk.hpp>

#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>

namespace Dusk {

PyObject * PyDusk_GetVersion(PyObject * self, PyObject * args) 
{
    const auto& version = GetVersion();
    PyObject * ret = PyTuple_New(3);
    PyTuple_SET_ITEM(ret, 0, PyLong_FromLong(version.Major));
    PyTuple_SET_ITEM(ret, 1, PyLong_FromLong(version.Minor));
    PyTuple_SET_ITEM(ret, 2, PyLong_FromLong(version.Patch));
    return ret;
}

PyObject * PyDusk_GetApplicationName(PyObject * self, PyObject * args) 
{
    const auto& name = GetApplicationName();
    return PyUnicode_FromString(name.c_str());
}

PyObject * PyDusk_GetApplicationVersion(PyObject * self, PyObject * args) 
{
    const auto& version = GetApplicationVersion();
    PyObject * ret = PyTuple_New(3);
    PyTuple_SET_ITEM(ret, 0, PyLong_FromLong(version.Major));
    PyTuple_SET_ITEM(ret, 1, PyLong_FromLong(version.Minor));
    PyTuple_SET_ITEM(ret, 2, PyLong_FromLong(version.Patch));
    return ret;
}

PyObject * PyDusk_LogVerbose(PyObject * self, PyObject * args)
{
#if defined(DUSK_ENABLE_VERBOSE_LOGGING)
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        return nullptr;
    }

    Log(LogLevel::Verbose, "[VERB](Py): %s\n", msg);
#endif

    Py_RETURN_NONE;
}

PyObject * PyDusk_LogInfo(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        return nullptr;
    }

    Log(LogLevel::Info, "[INFO](Py): %s\n", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogWarn(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        return nullptr;
    }

    Log(LogLevel::Warn, "[WARN](Py): %s\n", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogError(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        return nullptr;
    }

    Log(LogLevel::Error, "[ERRO](Py): %s\n", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogPerf(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        return nullptr;
    }

    Log(LogLevel::Performance, "[PERF](Py): %s\n", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogLoad(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        return nullptr;
    }

    Log(LogLevel::Load, "[LOAD](Py): %s\n", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LadModule(PyObject * self, PyObject * args)
{
    const char * module;

    if (!PyArg_ParseTuple(args, "s", &module)) {
        PyErr_BadArgument();
        return nullptr;
    }

    LoadModule(module);
    Py_RETURN_NONE;
}

PyMODINIT_FUNC PyInit_Dusk()
{
    PyObject * module = nullptr;

    module = PyModule_Create(&PyDuskModule);
    if (!module) {
        return nullptr;
    }

    // if (PyType_Ready(&Thing_type) == 0) {
    //     Py_INCREF(&Thing_type);
    //     if (PyModule_AddObject(module, "Thing", (PyObject *)&Thing_type) < 0) {
    //         Py_DECREF(&Thing_type);
    //         Py_DECREF(module);
    //         return nullptr;
    //     }
    // }

    return module;
}

} // namespace Dusk