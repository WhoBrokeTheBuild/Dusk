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
void RunScriptFile(const std::string& filename) {
    FILE * file = fopen(filename.c_str(), "rt");
    if (!file) {
        exit(1);
    }

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();

    PyRun_File(file, filename.c_str(), Py_file_input, pyMainDict, pyLocalDict);

    PyPrintStackTrace();

    fclose(file);
}

DUSK_CORE_API
void RunScriptString(const std::string& code)
{
    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();

    PyRun_String(code.c_str(), 0, pyMainDict, pyLocalDict);

    PyPrintStackTrace();
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
