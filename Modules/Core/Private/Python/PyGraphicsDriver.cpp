#include <Python/PyGraphicsDriver.hpp>
#include <Dusk/Log.hpp>

namespace Dusk {

PyObject * PyDusk_GetGraphicsDriver(PyObject * self, PyObject * args) {
    auto gfx = GetGraphicsDriver();
    printf("%p\n", gfx);
    if (!gfx) {
        PyErr_SetString(PyExc_RuntimeError, "No Graphics Driver found.");
        Py_RETURN_NONE;
    }

    PyGraphicsDriver * obj = (PyGraphicsDriver *)PyType_GenericNew(&PyGraphicsDriver_type, 0, 0);
    obj->Pointer = gfx;
    Py_INCREF(obj);
    return (PyObject *)obj;
}

} // namespace Dusk