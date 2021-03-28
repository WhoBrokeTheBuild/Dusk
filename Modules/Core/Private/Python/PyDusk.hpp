#ifndef DUSK_PY_DUSK_HPP
#define DUSK_PY_DUSK_HPP

#include <Dusk/Config.hpp>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

namespace Dusk {

PyObject * PyDusk_RunScriptFile(PyObject * self, PyObject * args);

PyObject * PyDusk_SetRunning(PyObject * self, PyObject * args);

PyObject * PyDusk_IsRunning(PyObject * self, PyObject * args);

PyObject * PyDusk_GetVersion(PyObject * self, PyObject * args);

PyObject * PyDusk_GetApplicationName(PyObject * self, PyObject * args);

PyObject * PyDusk_SetApplicationName(PyObject * self, PyObject * args);

PyObject * PyDusk_GetApplicationVersion(PyObject * self, PyObject * args);

PyObject * PyDusk_SetApplicationVersion(PyObject * self, PyObject * args);

PyObject * PyDusk_LoadModule(PyObject * self, PyObject * args);

PyMODINIT_FUNC PyInit_Dusk();

bool PyCheckError();

} // namespace Dusk

#endif // DUSK_PY_DUSK_HPP