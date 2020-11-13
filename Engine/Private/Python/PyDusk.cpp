#include <Python/PyDusk.hpp>

#include <Python/PyLog.hpp>
#include <Python/PyScriptEvent.hpp>
#include <Python/PyGraphicsDriver.hpp>

#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>

#include <string>

namespace Dusk {

PyObject * PyDusk_RunScriptFile(PyObject * self, PyObject * args)
{
    const char * filename;

    if (!PyArg_ParseTuple(args, "s", &filename)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    RunScriptFile(filename);
    Py_RETURN_NONE;
}

PyObject * PyDusk_SetRunning(PyObject * self, PyObject * args)
{
    bool running;

    if (!PyArg_ParseTuple(args, "b", &running)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    SetRunning(running);
    Py_RETURN_NONE;
}

PyObject * PyDusk_IsRunning(PyObject * self, PyObject * args)
{
    if (IsRunning()) {
        Py_RETURN_TRUE;
    }

    Py_RETURN_FALSE;
}

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

PyObject * PyDusk_SetApplicationName(PyObject * self, PyObject * args) 
{
    const char * name;

    if (!PyArg_ParseTuple(args, "s", &name)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    SetApplicationName(name);
    Py_RETURN_NONE;
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

PyObject * PyDusk_SetApplicationVersion(PyObject * self, PyObject * args) 
{
    Version version;

    if (!PyArg_ParseTuple(args, "(iii)", &version.Major, &version.Minor, &version.Patch)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    SetApplicationVersion(version);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LoadModule(PyObject * self, PyObject * args)
{
    const char * module;

    if (!PyArg_ParseTuple(args, "s", &module)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    LoadModule(module);
    Py_RETURN_NONE;
}

static struct PyMethodDef PyDusk_methods[] = {
    { "RunScriptFile",          PyDusk_RunScriptFile,           METH_VARARGS,   nullptr },
    { "SetRunning",             PyDusk_SetRunning,              METH_VARARGS,   nullptr },
    { "IsRunning",              PyDusk_IsRunning,               METH_NOARGS,    nullptr },
    { "GetVersion",             PyDusk_GetVersion,              METH_NOARGS,    "Get Dusk's version as a tuple, e.g. (1, 0, 0)" },
    { "GetApplicationName",     PyDusk_GetApplicationName,      METH_NOARGS,    nullptr },
    { "SetApplicationName",     PyDusk_SetApplicationName,      METH_VARARGS,   nullptr },
    { "GetApplicationVersion",  PyDusk_GetApplicationVersion,   METH_NOARGS,    nullptr },
    { "SetApplicationVersion",  PyDusk_SetApplicationVersion,   METH_VARARGS,   nullptr },
    { "LogVerbose",             PyDusk_LogVerbose,              METH_VARARGS,   nullptr },
    { "LogInfo",                PyDusk_LogInfo,                 METH_VARARGS,   nullptr },
    { "LogWarn",                PyDusk_LogWarn,                 METH_VARARGS,   nullptr },
    { "LogError",               PyDusk_LogError,                METH_VARARGS,   nullptr },
    { "LogPerf",                PyDusk_LogPerf,                 METH_VARARGS,   nullptr },
    { "LogLoad",                PyDusk_LogLoad,                 METH_VARARGS,   nullptr },
    { "LoadModule",             PyDusk_LoadModule,              METH_VARARGS,   nullptr },
    { "GetGraphicsDriver",      PyDusk_GetGraphicsDriver,       METH_NOARGS,    nullptr },
    { nullptr, nullptr, 0, nullptr },
};

static struct PyModuleDef PyDuskModule = {
    PyModuleDef_HEAD_INIT,
    "Dusk",                 // m_name
    "Dusk",                 // m_doc
    -1,                     // m_size
    PyDusk_methods,         // m_methods
};

PyMODINIT_FUNC PyInit_Dusk()
{
    PyObject * module = nullptr;

    module = PyModule_Create(&PyDuskModule);
    if (!module) {
        return nullptr;
    }

    if (!PyInit_ScriptEvent(module)) {
        return nullptr;
    }

    if (!PyInit_GraphicsDriver(module)) {
        return nullptr;
    }

    return module;
}

bool PyCheckError()
{
    if (PyErr_Occurred()) {
        PyObject * pyType = nullptr;
        PyObject * pyValue = nullptr;
        PyObject * pyTrace = nullptr;

        PyErr_Fetch(&pyType, &pyValue, &pyTrace);
        PyErr_NormalizeException(&pyType, &pyValue, &pyTrace);

        PyObject * pyValueRepr = PyObject_Repr(pyValue);
        if (pyValueRepr) {
            PyObject * pyValueStr = PyUnicode_AsEncodedString(pyValueRepr, "utf-8", "~E~");

            fprintf(stderr,
                "[ERRO] Exception %s\n", 
                PyBytes_AS_STRING(pyValueStr));

            Py_XDECREF(pyValueStr);
            Py_XDECREF(pyValueRepr);
        }
        else {
            PyObject * pyTypeName = PyObject_GetAttrString(pyType, "__name__");
            PyObject * pyTypeNameStr = PyUnicode_AsEncodedString(pyTypeName, "utf-8", "~E~");

            fprintf(stderr,
                "[ERRO] Exception %s\n", 
                PyBytes_AS_STRING(pyTypeNameStr));

            Py_XDECREF(pyTypeNameStr);
            Py_XDECREF(pyTypeName);
        }

        return true;
    }

    return false;
}

} // namespace Dusk