#include <Dusk/Module.hpp>

#if !defined(DUSK_PLATFORM_WINDOWS)

    #include <dlfcn.h>

#endif

#include <Dusk/Log.hpp>

#include <unordered_map>

namespace Dusk {
    
#if defined(DUSK_PLATFORM_WINDOWS)

    typedef HMODULE ModuleHandle;

#else

    typedef void * ModuleHandle;

#endif

std::unordered_map<string, ModuleHandle> _Modules;

ModuleHandle _dlopen(const string& filename)
{
    ModuleHandle handle = nullptr;

    #if defined(DUSK_PLATFORM_WINDOWS)

        LogVerbose(DUSK_ANCHOR, "Loading Module from PATH: '{}'", getenv("PATH"));

        handle = LoadLibraryA(filename.c_str());
        if (!handle) {
            fmt::windows_error msg(HRESULT_FROM_WIN32(GetLastError()));
            LogError(DUSK_ANCHOR, "Failed to load '%s', %s", filename, msg);
            return nullptr;
        }
        
    #else

        LogVerbose(DUSK_ANCHOR, "Loading Module from LD_LIBRARY_PATH: '{}'", getenv("LD_LIBRARY_PATH"));

        #if defined(DUSK_PLATFORM_APPLE)

            string libFilename = "lib" + filename + ".dylib";

        #else

            string libFilename = "lib" + filename + ".so";

        #endif

        handle = dlopen(libFilename.c_str(), RTLD_GLOBAL | RTLD_NOW);
        if (!handle) {
            LogError(DUSK_ANCHOR, "Failed to load '{}', {}", libFilename, dlerror());
            return nullptr;
        }

    #endif

    return handle;
}

void * _dlsym(ModuleHandle handle, const string& symbol)
{
    #if defined(DUSK_PLATFORM_WINDOWS)

        return GetProcAddress(handle, symbol.c_str());

    #else

        return dlsym(handle, symbol.c_str());

    #endif
}

void _dlclose(ModuleHandle handle)
{
    #if defined(DUSK_PLATFORM_WINDOWS)

        FreeLibrary(handle);

    #else

        dlclose(handle);

    #endif
}

bool LoadModule(const string& name, Version minVersion /*= Version()*/)
{
    LogVerbose(DUSK_ANCHOR, "Loading module '{}'", name);

    ModuleHandle handle = _dlopen(name);
    if (!handle) {
        return false;
    }

    DuskModuleDefinition * def = static_cast<DuskModuleDefinition *>(_dlsym(handle, "_DuskModule"));
    if (!def) {
        LogError(DUSK_ANCHOR, "Failed to find _DuskModule symbol");
        return false;
    }
    
    if (def->GetVersion) {
        Version ver = def->GetVersion();
        if (minVersion > ver) {
            _dlclose(handle);

            LogError(DUSK_ANCHOR, "Failed to load module '{}', found version {} but minimum version is {}", 
                name, ver, minVersion);
            return false;
        }

        LogVerbose(DUSK_ANCHOR, "Found module '{}' version {}", name, ver);
    }

    if (def->Initialize) {
        if (!def->Initialize()) {
            _dlclose(handle);

            LogError(DUSK_ANCHOR, "Failed to initialize module '{}'", name);
            return false;
        }
    }

    _Modules.emplace(name, handle);

    return true;
}

DUSK_ENGINE_API
void FreeModule(const string& name)
{
    auto it = _Modules.find(name);
    if (it == _Modules.end()) {
        LogWarning(DUSK_ANCHOR, "Failed to free module '{}', module is not loaded", name);
        return;
    }

    ModuleHandle handle = it->second;
    DuskModuleDefinition * def = static_cast<DuskModuleDefinition *>(_dlsym(handle, "_DuskModule"));

    if (def && def->Terminate) {
        def->Terminate();
    }

    _dlclose(handle);

    _Modules.erase(it);
}

void FreeAllModules()
{
    for (const auto& it : _Modules) {
        ModuleHandle handle = it.second;
        DuskModuleDefinition * def = static_cast<DuskModuleDefinition *>(_dlsym(handle, "_DuskModule"));
        if (def && def->Terminate) {
            def->Terminate();
        }

        _dlclose(handle);
    }

    _Modules.clear();
}

}
