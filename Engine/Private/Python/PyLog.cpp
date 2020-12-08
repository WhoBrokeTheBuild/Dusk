#include <Python/PyLog.hpp>

namespace Dusk {

PyObject * PyDusk_LogVerbose(PyObject * self, PyObject * args)
{
#if defined(DUSK_ENABLE_VERBOSE_LOGGING)
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        Py_RETURN_NONE;
    }

    Log(LogLevel::Verbose, "[VERB](Py): %s\n", msg);
#endif

    Py_RETURN_NONE;
}

PyObject * PyDusk_LogInfo(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    Log(LogLevel::Info, "[INFO](Py): %s\n", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogWarn(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    Log(LogLevel::Warning, "[WARN](Py): %s\n", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogError(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    Log(LogLevel::Error, "[ERRO](Py): %s\n", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogPerf(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    Log(LogLevel::Performance, "[PERF](Py): %s\n", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogLoad(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    Log(LogLevel::Load, "[LOAD](Py): %s\n", msg);
    Py_RETURN_NONE;
}

} // namespace Dusk