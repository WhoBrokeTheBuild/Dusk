#ifndef DUSK_INPUT_DRIVER_HPP
#define DUSK_INPUT_DRIVER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Keyboard.hpp>
#include <Dusk/Mouse.hpp>
#include <Dusk/Controller.hpp>

#include <memory>

namespace Dusk {

// class MouseMovedEventData : public EventData
// {
// public:

//     ivec2 Position;
//     ivec2 Delta;

//     PyObject * PySerialize() const override {
//         PyObject * dict = PyDict_New();

//         PyObject * pos = PyTuple_New(2);
//         PyTuple_SET_ITEM(pos, 0, PyLong_FromLong(Position.x));
//         PyTuple_SET_ITEM(pos, 1, PyLong_FromLong(Position.y));
//         PyDict_SetItemString(dict, "Position", pos);

//         PyObject * delta = PyTuple_New(2);
//         PyTuple_SET_ITEM(delta, 0, PyLong_FromLong(Delta.x));
//         PyTuple_SET_ITEM(delta, 1, PyLong_FromLong(Delta.y));
//         PyDict_SetItemString(dict, "Delta", delta);

//         return dict;
//     }

// }; // class MouseMovedEventData

class DUSK_CORE_API InputDriver : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(InputDriver)

    InputDriver() = default;
    
    virtual ~InputDriver() = default;

    virtual ivec2 GetMouseCoordinates() const = 0;

}; // class InputDriver

DUSK_CORE_API
void SetInputDriver(std::unique_ptr<InputDriver> && driver);

DUSK_CORE_API
InputDriver * GetInputDriver();

} // namespace Dusk

#endif // DUSK_INPUT_DRIVER_HPP
