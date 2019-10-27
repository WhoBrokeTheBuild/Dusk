#include "dusk/core/ScriptHost.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/scene/Scene.hpp>

#include <Python.h>
#include <frameobject.h>

#include <fstream>

namespace dusk {

static PyObject * PyDusk_Info(PyObject * self, PyObject * args)
{
    const char * message = nullptr;
    
    if (!PyArg_ParseTuple(args, "s", &message)) {
        Py_RETURN_NONE;
    }

    PyFrameObject * frame = PyEval_GetFrame();
    int line = PyFrame_GetLineNumber(frame);

    Log(LogLevel::LOG_INFO, "[INFO](Python:%d) %s\n", line, message);

    Py_RETURN_NONE;
}

static PyObject * PyDusk_Warn(PyObject * self, PyObject * args)
{
    const char * message = nullptr;

    if (!PyArg_ParseTuple(args, "s", &message)) {
        Py_RETURN_NONE;
    }

    PyFrameObject * frame = PyEval_GetFrame();
    int line = PyFrame_GetLineNumber(frame);

    Log(LogLevel::LOG_WARN, "[WARN](Python:%d) %s\n", line, message);

    Py_RETURN_NONE;
}

static PyObject * PyDusk_Error(PyObject * self, PyObject * args)
{
    const char * message = nullptr;

    if (!PyArg_ParseTuple(args, "s", &message)) {
        Py_RETURN_NONE;
    }

    PyFrameObject * frame = PyEval_GetFrame();
    int line = PyFrame_GetLineNumber(frame);

    Log(LogLevel::LOG_ERROR, "[ERRO](Python:%d) %s\n", line, message);

    Py_RETURN_NONE;
}

static PyObject * PyDusk_Perf(PyObject * self, PyObject * args)
{
    const char * message = nullptr;

    if (!PyArg_ParseTuple(args, "s", &message)) {
        Py_RETURN_NONE;
    }

    PyFrameObject * frame = PyEval_GetFrame();
    int line = PyFrame_GetLineNumber(frame);

    Log(LogLevel::LOG_PERF, "[PERF](Python:%d) %s\n", line, message);

    Py_RETURN_NONE;
}

static PyObject * PyDusk_Verbose(PyObject * self, PyObject * args)
{
    const char * message = nullptr;

    if (!PyArg_ParseTuple(args, "s", &message)) {
        Py_RETURN_NONE;
    }

    PyFrameObject * frame = PyEval_GetFrame();
    int line = PyFrame_GetLineNumber(frame);

    Log(LogLevel::LOG_VERBOSE, "[VERB](Python:%d) %s\n", line, message);

    Py_RETURN_NONE;
}

static PyObject * PyDusk_Load(PyObject * self, PyObject * args)
{
    const char * message = nullptr;

    if (!PyArg_ParseTuple(args, "s", &message)) {
        Py_RETURN_NONE;
    }

    PyFrameObject * frame = PyEval_GetFrame();
    int line = PyFrame_GetLineNumber(frame);

    Log(LogLevel::LOG_LOAD, "[LOAD](Python:%d) %s\n", line, message);

    Py_RETURN_NONE;
}

static PyMethodDef PyDuskMethods[] = {
    { "Info",       PyDusk_Info,        METH_VARARGS, nullptr },
    { "Warn",       PyDusk_Warn,        METH_VARARGS, nullptr },
    { "Error",      PyDusk_Error,       METH_VARARGS, nullptr },
    { "Perf",       PyDusk_Perf,        METH_VARARGS, nullptr },
    { "Verbose",    PyDusk_Verbose,     METH_VARARGS, nullptr },
    { "Load",       PyDusk_Load,        METH_VARARGS, nullptr },
    { nullptr, nullptr, 0, nullptr },
};

static struct PyModuleDef PyDuskModule = {
    PyModuleDef_HEAD_INIT,
    "Dusk",
    nullptr,
    -1,
    PyDuskMethods,
};

PyMODINIT_FUNC PyInit_Dusk()
{
    PyObject * module = nullptr;

    module = PyModule_Create(&PyDuskModule);
    if (!module) {
        return nullptr;
    }

    return module;
}

ScriptHost::ScriptHost()
{
    PyImport_AppendInittab("Dusk", PyInit_Dusk);
    Py_Initialize();
    PyImport_ImportModule("Dusk");
}

ScriptHost::~ScriptHost()
{
    Py_Finalize();
}

bool ScriptHost::RunFile(const std::string& filename)
{
    const auto& paths = GetAssetPaths();

    std::string fullPath;
    FILE * file = nullptr;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath);
        file = fopen(fullPath.c_str(), "rt");

        if (file) break;
    }

    if (!file) {
        DuskLogError("Could not open file '%s'", filename);
        return false;
    }
    
    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();
    PyDict_SetItemString(pyLocalDict, "__file__", PyBytes_FromString(fullPath.c_str()));

    PyRun_File(file, fullPath.c_str(), Py_file_input, pyMainDict, pyLocalDict);

    if (PyErr_Occurred()) {
        PyObject * pyType = nullptr;
        PyObject * pyValue = nullptr;
        PyObject * pyTrace = nullptr;

        PyErr_Fetch(&pyType, &pyValue, &pyTrace);
        PyErr_NormalizeException(&pyType, &pyValue, &pyTrace);

        PyObject * pyValueRepr = PyObject_Repr(pyValue);
        if (pyValueRepr) {
            PyObject * pyValueStr = PyUnicode_AsEncodedString(pyValueRepr, "utf-8", "~E~");

            Log(LogLevel::LOG_ERROR, 
                "[ERRO](%s) Exception %s\n", 
                GetBasename(fullPath),
                PyBytes_AS_STRING(pyValueStr));

            Py_XDECREF(pyValueStr);
            Py_XDECREF(pyValueRepr);
        }
        else {
            PyObject * pyTypeName = PyObject_GetAttrString(pyType, "__name__");
            PyObject * pyTypeNameStr = PyUnicode_AsEncodedString(pyTypeName, "utf-8", "~E~");

            Log(LogLevel::LOG_ERROR, 
                "[ERRO](%s) Exception %s\n", 
                GetBasename(fullPath),
                PyBytes_AS_STRING(pyTypeNameStr));

            Py_XDECREF(pyTypeNameStr);
            Py_XDECREF(pyTypeName);
        }
    }

    fclose(file);
    file = nullptr;

    return true;
}

} // namespace dusk
