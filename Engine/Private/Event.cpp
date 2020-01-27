#include <Dusk/Event.hpp>

#include <Python/PyDusk.hpp>

namespace Dusk {

unsigned ScriptEvent::AddPythonListener(PyObject * listener)
{
    if (!PyCallable_Check(listener)) {
        DuskLogError("PyObject not callable");
        return 0;
    }

    ++_maxPythonListenerID;
    Py_INCREF(listener);
    _pythonListeners.emplace(_maxPythonListenerID, listener);

    return _maxPythonListenerID;
}

bool ScriptEvent::RemovePythonListener(PyObject * listener)
{
    auto it = _pythonListeners.begin();
    while (it != _pythonListeners.end()) {
        if (it->second == listener) {
            break;
        }
    }

    if (it != _pythonListeners.end()) {
        _pythonListeners.erase(it);
        return true;
    }

    return false;
}

bool ScriptEvent::RemovePythonListener(unsigned id)
{
    auto it = _pythonListeners.find(id);

    if (it != _pythonListeners.end()) {
        _pythonListeners.erase(it);
        return true;
    }

    return false;
}

void ScriptEvent::CallPython(const EventData * data)
{
    PyObject * dict = data->GetPyObject();
    PyObject * arg = Py_BuildValue("(O)", dict);

    DuskLogInfo("Calling Python Listeners");
    for (const auto& it : _pythonListeners) {
        PyObject * result = PyEval_CallObject(it.second, arg);
        if (!result) {
            DuskLogError("Failed to call Python Event Listener");
            PyPrintStackTrace();
        }
    }

    Py_DECREF(arg);
    Py_DECREF(dict);
}

void ScriptEvent::RemoveAllPythonListeners()
{
    for (const auto& it : _pythonListeners) {
        Py_DECREF(it.second);
    }
    _pythonListeners.clear();
}

} // namespace Dusk
