#ifndef DUSK_EVENT_HPP
#define DUSK_EVENT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Log.hpp>

#include <Python.h>

#include <unordered_map>
#include <functional>
#include <type_traits>

namespace Dusk {

class EventData
{
public:

    template <class T>
    const T * GetAs() const {
        return dynamic_cast<const T *>(this);
    }

    virtual PyObject * GetPyObject() const = 0;

}; // class EventData

class ScriptEvent
{
public:

    DISALLOW_COPY_AND_ASSIGN(ScriptEvent)

    explicit ScriptEvent() = default;

    virtual ~ScriptEvent() = default;

    unsigned AddPythonListener(PyObject * listener);

    bool RemovePythonListener(PyObject * listener);

    bool RemovePythonListener(unsigned id);

    void Call(const EventData * data);

    void RemoveAllPythonListeners();

private:

    unsigned _maxPythonListenerID = 0;

    std::unordered_map<unsigned, PyObject *> _pythonListeners;

}; // class ScriptEvent

template <class EventDataType>
class Event : public ScriptEvent
{
    static_assert(
        std::is_base_of<EventData, EventDataType>::value,
        "EventDataType must be a descendant of EventData"
    );

public:

    DISALLOW_COPY_AND_ASSIGN(Event)

    explicit Event() = default;

    virtual ~Event() = default;

    typedef std::function<void(const EventDataType *)> Listener;

    void AddListener(Listener listener) {
        ++_maxListenerID;
        _listeners.emplace(_maxListenerID, listener);
        return _maxListenerID;
    }

    bool RemoveListener(unsigned id) {
        auto it = _listeners.find(id);

        if (it != _listeners.end()) {
            _listeners.erase(it);
            return true;
        }

        return false;
    }

    void Call(const EventDataType * data) {
        for (const auto& it : _listeners) {
            it.second(data);
        }

        ScriptEvent::Call(static_cast<const EventData *>(data));
    }

    void RemoveAllListeners() {
        _listeners.clear();

        RemoveAllPythonListeners();
    }

private:

    unsigned _maxListenerID;

    std::unordered_map<int, Listener> _listeners;

}; // class Event<EventDataType>

} // namespace Dusk

#endif // DUSK_EVENT_HPP
