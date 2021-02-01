#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/Time.hpp>
#include <Dusk/Scene.hpp>

#include <Python/PyDusk.hpp>

#include <Python.h>
#include <frameobject.h>

#include <string>
#include <vector>
#include <cstdio>
#include <thread>

namespace Dusk {

DUSK_ENGINE_API
bool Initialize(int argc, char ** argv)
{
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

    return true;
}

DUSK_ENGINE_API
void Terminate()
{
    Py_Finalize();

    FreeAllModules();

    TermMemoryTracking();
    
    CloseAllLogFiles();
}

static bool _Running = false;

DUSK_ENGINE_API
void SetRunning(bool running)
{
    _Running = running;
}

DUSK_ENGINE_API
bool IsRunning()
{
    return _Running;
}

DUSK_ENGINE_API
bool Run(std::function<void()> update)
{
    using namespace std::chrono;

    GraphicsDriver * gfx = GetGraphicsDriver();
    UpdateContext * updateCtx = gfx->GetUpdateContext();

    high_resolution_clock::time_point startTime = high_resolution_clock::now();
    high_resolution_clock::time_point previousTime = startTime;


    Dusk::SetRunning(true);

    while (Dusk::IsRunning()) {
        high_resolution_clock::time_point currentTime = high_resolution_clock::now();
        auto totalDuration = duration_cast<milliseconds>(currentTime - startTime);
        auto previousFrameDuration = duration_cast<microseconds>(currentTime - previousTime);
        previousTime = currentTime;

        updateCtx->SetTotalDuration(totalDuration);
        updateCtx->SetPreviousFrameDuration(previousFrameDuration);
        auto frameEndTime = currentTime + updateCtx->GetExpectedFrameDuration();

        gfx->ProcessEvents();

        update();

        auto scene = GetCurrentScene();
        if (scene) {
            scene->Update(gfx->GetUpdateContext());
        }

        gfx->Render();

        currentTime = high_resolution_clock::now();
        auto timeToSleep = duration_cast<milliseconds>(frameEndTime - currentTime);
        if (timeToSleep > 1ms) { // TODO: Find "minimum" sleep time
            std::this_thread::sleep_for(timeToSleep);
        }
    }

    return true;
}

DUSK_ENGINE_API
bool RunScriptFile(const std::string& filename)
{
    FILE * file = nullptr;
    
    const auto& assetPaths = GetAssetPathList();

    for (const auto& path : assetPaths) {
        const std::string& fullPath = path + "Scripts/" + filename;
        DuskLogVerbose("Checking '%s'", fullPath);

        file = fopen(fullPath.c_str(), "rt");
        if (file) {
            break;
        }
    }

    if (!file) {
        DuskLogError("Failed to script '%s'", filename);
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
