#include <Python/PyDusk.hpp>

#include <Python/PyLog.hpp>
#include <Python/PyScriptEvent.hpp>
#include <Python/PyGraphicsDriver.hpp>

#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>

namespace Dusk {

PyObject * PyDusk_RunScriptFile(PyObject * self, PyObject * args)
{
    const char * filename;
    int useAssetPath = true;

    if (!PyArg_ParseTuple(args, "s|p", &filename, &useAssetPath)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    RunScriptFile(filename, useAssetPath);
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
    Version ver;

    if (!PyArg_ParseTuple(args, "(iii)",  &ver.Major, &ver.Minor, &ver.Patch)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    SetApplicationVersion(ver);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LoadModule(PyObject * self, PyObject * args)
{
    const char * name;
    Version minVer = Version();

    if (!PyArg_ParseTuple(args, "s|(iii)", &name, &minVer.Major, &minVer.Minor, &minVer.Patch)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    return PyBool_FromLong(LoadModule(name, minVer));
}

PyObject * PyDusk_FreeModule(PyObject * self, PyObject * args)
{
    const char * name;

    if (!PyArg_ParseTuple(args, "s", &name)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    FreeModule(name);
    Py_RETURN_NONE;
}

static struct PyMethodDef PyDusk_methods[] = {
    { "RunScriptFile",              PyDusk_RunScriptFile,               METH_VARARGS,   "Dusk::RunScriptFile(filename)" },
    { "SetRunning",                 PyDusk_SetRunning,                  METH_VARARGS,   "Dusk::SetRunning(running)" },
    { "IsRunning",                  PyDusk_IsRunning,                   METH_NOARGS,    "Dusk::IsRunning()" },
    { "GetVersion",                 PyDusk_GetVersion,                  METH_NOARGS,    "Dusk::GetVersion(), returns a tuple of (Major, Minor, Patch)" },
    { "GetApplicationName",         PyDusk_GetApplicationName,          METH_NOARGS,    "Dusk::GetApplicationName()" },
    { "SetApplicationName",         PyDusk_SetApplicationName,          METH_VARARGS,   "Dusk::SetApplicationName(string)" },
    { "GetApplicationVersion",      PyDusk_GetApplicationVersion,       METH_NOARGS,    "Dusk::GetApplicationVersion(), returns a tuple of (Major, Minor, Patch)" },
    { "SetApplicationVersion",      PyDusk_SetApplicationVersion,       METH_VARARGS,   "Dusk::SetApplicationVersion(version), version must be a tuple of (Major, Minor, Patch)" },
    { "SetMinimumLogLevel",         PyDusk_SetMinimumLogLevel,          METH_VARARGS,   "Dusk::SetMinimumLogLevel(level)" },
    { "GetMinimumLogLevel",         PyDusk_GetMinimumLogLevel,          METH_NOARGS,    "Dusk::GetMinimumLogLevel()" },
    { "LogVerbose",                 PyDusk_LogVerbose,                  METH_VARARGS,   "Dusk::LogVerbose()" },
    { "LogPerformance",             PyDusk_LogPerformance,              METH_VARARGS,   "Dusk::LogPerformance()" },
    { "LogDebug",                   PyDusk_LogDebug,                    METH_VARARGS,   "Dusk::LogDebug()" },
    { "LogInfo",                    PyDusk_LogInfo,                     METH_VARARGS,   "Dusk::LogInfo()" },
    { "LogWarning",                 PyDusk_LogWarning,                  METH_VARARGS,   "Dusk::LogWarning()" },
    { "LogError",                   PyDusk_LogError,                    METH_VARARGS,   "Dusk::LogError()" },
    { "LogFatal",                   PyDusk_LogFatal,                    METH_VARARGS,   "Dusk::LogFatal()" },
    { "LoadModule",                 PyDusk_LoadModule,                  METH_VARARGS,   "Dusk::LoadModule(name, [minVersion]), minVersion must be a tuple of (Major, Minor, Patch)" },
    { "FreeModule",                 PyDusk_FreeModule,                  METH_VARARGS,   "Dusk::FreeModule(name)" },
    // { "GetGraphicsDriver",          PyDusk_GetGraphicsDriver,           METH_NOARGS,    "Dusk::GetGraphicsDriver()" },
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
    PyObject * mod = nullptr;

    mod = PyModule_Create(&PyDuskModule);
    if (!mod) {
        return nullptr;
    }

    if (!PyInit_ScriptEvent(mod)) {
        return nullptr;
    }

    if (!PyInit_GraphicsDriver(mod)) {
        return nullptr;
    }

    return mod;
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

            LogError("Python", "Exception {}", PyBytes_AS_STRING(pyValueStr));

            Py_XDECREF(pyValueStr);
            Py_XDECREF(pyValueRepr);
        }
        else {
            PyObject * pyTypeName = PyObject_GetAttrString(pyType, "__name__");
            PyObject * pyTypeNameStr = PyUnicode_AsEncodedString(pyTypeName, "utf-8", "~E~");

            LogError("Python", "Exception {}", PyBytes_AS_STRING(pyTypeNameStr));

            Py_XDECREF(pyTypeNameStr);
            Py_XDECREF(pyTypeName);
        }

        return true;
    }

    return false;
}

} // namespace Dusk