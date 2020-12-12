#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/GraphicsDriver.hpp>

#include <Python/PyDusk.hpp>

#include <Python.h>
#include <frameobject.h>

#include <string>
#include <vector>
#include <cstdio>

namespace Dusk {

DUSK_ENGINE_API
void Initialize(int argc, char ** argv) {
    InitMemoryTracking();

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

DUSK_ENGINE_API
void Terminate() {
    Py_Finalize();

    FreeAllModules();

    TermMemoryTracking();
}

static bool _Running = false;

DUSK_ENGINE_API
void SetRunning(bool running) {
    _Running = running;
}

DUSK_ENGINE_API
bool IsRunning() {
    return _Running;
}

DUSK_ENGINE_API
bool RunScriptFile(const std::string& filename) {
    FILE * file = fopen(filename.c_str(), "rt");
    if (!file) {
        return false;
    }

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();

    PyRun_File(file, filename.c_str(), Py_file_input, pyMainDict, pyLocalDict);

    PyCheckError();

    fclose(file);
    return true;
}

DUSK_ENGINE_API
bool RunScriptString(const std::string& code)
{
    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();

    PyRun_String(code.c_str(), Py_single_input, pyMainDict, pyLocalDict);

    PyCheckError();
    return true;
}

DUSK_ENGINE_API
Version GetVersion()
{
    return Version(DUSK_VERSION_MAJOR, DUSK_VERSION_MINOR, DUSK_VERSION_PATCH);
}

static std::string _ApplicationName;

DUSK_ENGINE_API
void SetApplicationName(const std::string& name)
{
    _ApplicationName = name;
}

DUSK_ENGINE_API
std::string GetApplicationName()
{
    return _ApplicationName;
}

static Version _ApplicationVersion;

DUSK_ENGINE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch)
{
    _ApplicationVersion = Version(major, minor, patch);
}

DUSK_ENGINE_API
void SetApplicationVersion(const Version& version)
{
    _ApplicationVersion = version;
}

DUSK_ENGINE_API
Version GetApplicationVersion()
{
    return _ApplicationVersion;
}

}
