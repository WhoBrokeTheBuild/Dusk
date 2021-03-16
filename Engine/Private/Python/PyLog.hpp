#ifndef DUSK_PY_LOG_HPP
#define DUSK_PY_LOG_HPP

#include "Python/PyDusk.hpp"
#include <Dusk/Log.hpp>

namespace Dusk {

PyObject * PyDusk_SetMinimumLogLevel(PyObject * self, PyObject * args);

PyObject * PyDusk_GetMinimumLogLevel(PyObject * self, PyObject * args);

PyObject * PyDusk_LogVerbose(PyObject * self, PyObject * args);

PyObject * PyDusk_LogPerformance(PyObject * self, PyObject * args);

PyObject * PyDusk_LogDebug(PyObject * self, PyObject * args);

PyObject * PyDusk_LogInfo(PyObject * self, PyObject * args);

PyObject * PyDusk_LogWarning(PyObject * self, PyObject * args);

PyObject * PyDusk_LogError(PyObject * self, PyObject * args);

PyObject * PyDusk_LogFatal(PyObject * self, PyObject * args);

} // namespace Dusk

#endif // DUSK_PY_LOG_HPP