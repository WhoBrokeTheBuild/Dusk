#include <Python/PyLog.hpp>

namespace Dusk {

PyObject * PyDusk_SetMinimumLogLevel(PyObject * self, PyObject * args)
{
    long level = 0;

    if (!PyArg_ParseTuple(args, "l", &level)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    SetMinimumLogLevel(static_cast<LogLevel>(level));
    Py_RETURN_NONE;
}

PyObject * PyDusk_GetMinimumLogLevel(PyObject * self, PyObject * args)
{
    LogLevel level = GetMinimumLogLevel();
    return PyLong_FromLong(static_cast<long>(level));
}

PyObject * PyDusk_LogVerbose(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        Py_RETURN_NONE;
    }

    Log(LogLevel::Verbose, "Python", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogPerformance(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    Log(LogLevel::Performance, "Python", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogDebug(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        Py_RETURN_NONE;
    }

    Log(LogLevel::Debug, "Python", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogInfo(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    Log(LogLevel::Info, "Python", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogWarning(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    Log(LogLevel::Warning, "Python", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogError(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    Log(LogLevel::Error, "Python", msg);
    Py_RETURN_NONE;
}

PyObject * PyDusk_LogFatal(PyObject * self, PyObject * args)
{
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    Log(LogLevel::Fatal, "Python", msg);
    std::terminate();
}

} // namespace Dusk