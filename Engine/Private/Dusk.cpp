#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>

#include <Python/PyDusk.hpp>

#include <Python.h>
#include <frameobject.h>

#include <cstdio>

namespace Dusk {

DUSK_CORE_API
void Initialize(int argc, char ** argv) {
    PyImport_AppendInittab("Dusk", PyInit_Dusk);

    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program) {
        Py_SetProgramName(program);
    }

    PyMem_RawFree(program);
    
    Py_Initialize();
    PyImport_ImportModule("Dusk");

    DuskLogVerbose("Dusk Version: %s", GetVersion().GetString());
    DuskLogVerbose("Application Name: %s", GetApplicationName());
    DuskLogVerbose("Application Version: %s", GetApplicationVersion().GetString());
}

DUSK_CORE_API
void Terminate() {
    Py_Finalize();

    FreeModules();
}

static bool _Running = false;

DUSK_CORE_API
void SetRunning(bool running) {
    _Running = running;
}

DUSK_CORE_API
bool IsRunning() {
    return _Running;
}

DUSK_CORE_API
void RunScript(const std::string& filename) {
    FILE * file = fopen(filename.c_str(), "rt");
    if (!file) {
        exit(1);
    }

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();

    PyRun_File(file, filename.c_str(), Py_file_input, pyMainDict, pyLocalDict);

    if (PyErr_Occurred()) {
        PyObject * pyType = nullptr;
        PyObject * pyValue = nullptr;
        PyObject * pyTrace = nullptr;

        PyErr_Fetch(&pyType, &pyValue, &pyTrace);
        PyErr_NormalizeException(&pyType, &pyValue, &pyTrace);

        PyObject * pyValueRepr = PyObject_Repr(pyValue);
        if (pyValueRepr) {
            PyObject * pyValueStr = PyUnicode_AsEncodedString(pyValueRepr, "utf-8", "~E~");

            fprintf(stderr,
                "[ERRO] Exception %s\n", 
                PyBytes_AS_STRING(pyValueStr));

            Py_XDECREF(pyValueStr);
            Py_XDECREF(pyValueRepr);
        }
        else {
            PyObject * pyTypeName = PyObject_GetAttrString(pyType, "__name__");
            PyObject * pyTypeNameStr = PyUnicode_AsEncodedString(pyTypeName, "utf-8", "~E~");

            fprintf(stderr,
                "[ERRO] Exception %s\n", 
                PyBytes_AS_STRING(pyTypeNameStr));

            Py_XDECREF(pyTypeNameStr);
            Py_XDECREF(pyTypeName);
        }
    }

    fclose(file);
}

static std::string _ApplicationName;

DUSK_CORE_API
void SetApplicationName(const std::string& name)
{
    _ApplicationName = name;
}

DUSK_CORE_API
std::string GetApplicationName()
{
    return _ApplicationName;
}

static Version _ApplicationVersion;

DUSK_CORE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch)
{
    _ApplicationVersion = Version(major, minor, patch);
}

DUSK_CORE_API
void SetApplicationVersion(const Version& version)
{
    _ApplicationVersion = version;
}

DUSK_CORE_API
Version GetApplicationVersion()
{
    return _ApplicationVersion;
}

}
