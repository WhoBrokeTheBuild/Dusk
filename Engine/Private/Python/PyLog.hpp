#ifndef DUSK_PY_LOG_HPP
#define DUSK_PY_LOG_HPP

#include <Dusk/Log.hpp>

#include <Python.h>

namespace Dusk {
    
PyObject * PyDusk_LogVerbose(PyObject * self, PyObject * args);

PyObject * PyDusk_LogInfo(PyObject * self, PyObject * args);

PyObject * PyDusk_LogWarn(PyObject * self, PyObject * args);

PyObject * PyDusk_LogError(PyObject * self, PyObject * args);

PyObject * PyDusk_LogFatal(PyObject * self, PyObject * args);

PyObject * PyDusk_LogPerf(PyObject * self, PyObject * args);

PyObject * PyDusk_LogLoad(PyObject * self, PyObject * args);

} // namespace Dusk

#endif // DUSK_PY_LOG_HPP