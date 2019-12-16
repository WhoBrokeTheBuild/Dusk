#ifndef DUSK_PY_DUSK_HPP
#define DUSK_PY_DUSK_HPP

#include <Python/PyGraphicsDriver.hpp>

#include <Python.h>

namespace Dusk {

PyObject * PyDusk_GetVersion(PyObject * self, PyObject * args);

PyObject * PyDusk_GetApplicationName(PyObject * self, PyObject * args);

PyObject * PyDusk_GetApplicationVersion(PyObject * self, PyObject * args);

PyObject * PyDusk_LogVerbose(PyObject * self, PyObject * args);

PyObject * PyDusk_LogInfo(PyObject * self, PyObject * args);

PyObject * PyDusk_LogWarn(PyObject * self, PyObject * args);

PyObject * PyDusk_LogError(PyObject * self, PyObject * args);

PyObject * PyDusk_LogPerf(PyObject * self, PyObject * args);

PyObject * PyDusk_LogLoad(PyObject * self, PyObject * args);

PyObject * PyDusk_LadModule(PyObject * self, PyObject * args);

static struct PyMethodDef PyDusk_methods[] = {
    { "GetVersion",             PyDusk_GetVersion,              METH_NOARGS,    nullptr },
    { "GetApplicationName",     PyDusk_GetApplicationName,      METH_NOARGS,    nullptr },
    { "GetApplicationVersion",  PyDusk_GetApplicationVersion,   METH_NOARGS,    nullptr },
    { "LogVerbose",             PyDusk_LogVerbose,              METH_VARARGS,   nullptr },
    { "LogInfo",                PyDusk_LogInfo,                 METH_VARARGS,   nullptr },
    { "LogWarn",                PyDusk_LogWarn,                 METH_VARARGS,   nullptr },
    { "LogError",               PyDusk_LogError,                METH_VARARGS,   nullptr },
    { "LogPerf",                PyDusk_LogPerf,                 METH_VARARGS,   nullptr },
    { "LogLoad",                PyDusk_LogLoad,                 METH_VARARGS,   nullptr },
    { "LoadModule",             PyDusk_LadModule,               METH_VARARGS,   nullptr },
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

PyMODINIT_FUNC PyInit_Dusk();

} // namespace Dusk

#endif // DUSK_PY_DUSK_HPP