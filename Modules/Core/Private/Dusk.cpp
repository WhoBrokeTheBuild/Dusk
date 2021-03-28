#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/Time.hpp>
#include <Dusk/Scene.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/Version.hpp>
#include <Dusk/Exception.hpp>
#include <Dusk/JSON.hpp>

#include <Python/PyDusk.hpp>

#include <Python.h>
#include <frameobject.h>

#include <string>
#include <vector>
#include <cstdio>
#include <thread>
#include <fstream>

#include <cflags.h>

namespace Dusk {

DUSK_CORE_API
bool Initialize(int argc, char ** argv)
{
    cflags_t * flags = cflags_init();

    bool help = false;
    cflags_add_bool(flags, '\0', "help", &help, "Display this help and exit");

    bool verbose = false;
    cflags_add_bool(flags, 'v', "verbose", &verbose, "Enable verbose logging");

    const char * config = NULL;
    cflags_add_string(flags, 'c', "config", &config, "Configuration to use");

    cflags_parse(flags, argc, argv);

    if (help) {
        cflags_print_usage(flags,
            "[OPTION]...",
            "Dusk game and simulation engine",
            "Additional information about this library can be found at:\n"
            "  https://github.com/WhoBrokeTheBuild/Dusk"
        );
    }

    if (flags->argc > 1) {
        LoadConfigurationFile(flags->argv[1], (config ? config : ""));
    }

    InitMemoryTracking();

    PyImport_AppendInittab("Dusk", PyInit_Dusk);

    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program) {
        Py_SetProgramName(program);
    }

    PyMem_RawFree(program);
    
    Py_Initialize();
    PyImport_ImportModule("Dusk");

    RunScriptFile("Dusk/Dusk.py");

    LogVerbose(DUSK_ANCHOR, "Dusk Version: {}", GetVersion());
    LogVerbose(DUSK_ANCHOR, "Application Name: {}", GetApplicationName());
    LogVerbose(DUSK_ANCHOR, "Application Version: {}", GetApplicationVersion());
    LogVerbose(DUSK_ANCHOR, "Current Path: {}", GetCurrentPath());

    return true;
}

DUSK_CORE_API
void Terminate()
{
    Py_Finalize();

    FreeAllModules();

    TermMemoryTracking();
    
    CloseAllLogFiles();
}

DUSK_CORE_API
void LoadConfigurationFile(const Path& path, string_view configName)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw FileNotFound(path);
    }

    json data;
    data << file;

    auto applyConfig = [](json& data) {
        const auto& modules = data.value<std::vector<string>>("modules", {});
        for (const auto& name : modules) {
            LoadModule(name);
        }
    };

    SetApplicationName(data.value("name", ""));
    SetApplicationVersion(data.value("version", ""));

    applyConfig(data);

    if (!configName.empty()) {
        auto it = data.find("configurations");
        if (it != data.end()) {
            auto& configurations = it.value();
            if (configurations.is_object()) {
                it = configurations.find(configName);
                if (it != configurations.end()) {
                    applyConfig(it.value());
                }
            }
        }
    }
}

static bool _Running = false;

DUSK_CORE_API
void SetRunning(bool running)
{
    _Running = running;
}

DUSK_CORE_API
bool IsRunning()
{
    return _Running;
}

DUSK_CORE_API
bool Run(std::function<void()> update)
{
    using namespace std::chrono;

    auto gfx = GraphicsDriver::GetInstance();
    assert(gfx);

    UpdateContext * updateCtx = gfx->GetUpdateContext();
    assert(updateCtx);

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

DUSK_CORE_API
bool RunScriptFile(const Path& path, bool useAssetPath /*= true*/)
{
    FILE * file = nullptr;
    
    if (useAssetPath) {
        for (const auto& assetPath : GetAssetPathList()) {
            Path fullPath = assetPath / "Scripts" / path;
            LogVerbose(DUSK_ANCHOR, "Checking '{}'", fullPath);

            file = fopen(fullPath.ToCString(), "rt");
            if (file) {
                break;
            }
        }
    }
    else {
        file = fopen(path.ToCString(), "rt");
    }

    if (!file) {
        LogError(DUSK_ANCHOR, "Failed to run script file '{}'", path);
        return false;
    }

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();

    PyRun_File(file, path.ToCString(), Py_file_input, pyMainDict, pyLocalDict);

    PyCheckError();

    fclose(file);
    return true;
}

DUSK_CORE_API
bool RunScriptString(const string& code)
{
    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();

    PyRun_String(code.c_str(), Py_single_input, pyMainDict, pyLocalDict);

    PyCheckError();
    return true;
}

DUSK_CORE_API
Version GetVersion()
{
    return Version(DUSK_VERSION_MAJOR, DUSK_VERSION_MINOR, DUSK_VERSION_PATCH);
}

static string _ApplicationName;

DUSK_CORE_API
void SetApplicationName(const string& name)
{
    _ApplicationName = name;
}

DUSK_CORE_API
string GetApplicationName()
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
