#ifndef DUSK_PY_LOG_HPP
#define DUSK_PY_LOG_HPP

#include "Python/PyDusk.hpp"
#include <Dusk/Log.hpp>

namespace Dusk {

PyObject * PyDusk_SetVerboseLoggingEnabled(PyObject * self, PyObject * args);

PyObject * PyDusk_IsVerboseLoggingEnabled(PyObject * self, PyObject * args);
    
PyObject * PyDusk_LogVerbose(PyObject * self, PyObject * args);

PyObject * PyDusk_LogInfo(PyObject * self, PyObject * args);

PyObject * PyDusk_LogWarn(PyObject * self, PyObject * args);

PyObject * PyDusk_LogError(PyObject * self, PyObject * args);

PyObject * PyDusk_LogFatal(PyObject * self, PyObject * args);

PyObject * PyDusk_LogPerf(PyObject * self, PyObject * args);

PyObject * PyDusk_LogLoad(PyObject * self, PyObject * args);

} // namespace Dusk

#endif // DUSK_PY_LOG_HPP