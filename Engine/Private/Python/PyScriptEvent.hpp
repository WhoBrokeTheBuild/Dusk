#ifndef DUSK_PY_SCRIPT_EVENT_HPP
#define DUSK_PY_SCRIPT_EVENT_HPP

#include <Dusk/Python/PyDusk.hpp>
#include <Dusk/Event.hpp>

namespace Dusk {

PyObject * New_PyScriptEvent(ScriptEvent * evt);

PyObject * PyScriptEvent_AddListener(PyObject * self, PyObject * args);

PyObject * PyScriptEvent_RemoveAllListeners(PyObject * self, PyObject * args);

// PyObject * PyDusk_Call(PyObject * self, PyObject * args);

bool PyInit_ScriptEvent(PyObject * mod);

} // namespace Dusk

#endif // DUSK_PY_SCRIPT_EVENT_HPP