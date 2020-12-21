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

std::unordered_map<std::string, ModuleHandle> _Modules;

ModuleHandle _dlopen(const std::string& filename)
{
    ModuleHandle handle = nullptr;

    #if defined(DUSK_PLATFORM_WINDOWS)

        DuskLogVerbose("Loading Module from PATH: '%s'", getenv("PATH"));

        handle = LoadLibraryA(filename.c_str());
        if (!handle) {
            WindowsErrorMessage msg(HRESULT_FROM_WIN32(GetLastError()));
            DuskLogError("Failed to load '%s', %s", filename, msg);
            return nullptr;
        }
        
    #else

        DuskLogVerbose("Loading Module from LD_LIBRARY_PATH: '%s'", getenv("LD_LIBRARY_PATH"));

        #if defined(DUSK_PLATFORM_APPLE)

            std::string libFilename = "lib" + filename + ".dylib";

        #else

            std::string libFilename = "lib" + filename + ".so";

        #endif

        handle = dlopen(libFilename.c_str(), RTLD_GLOBAL | RTLD_NOW);
        if (!handle) {
            DuskLogError("Failed to load '%s', %s", libFilename, dlerror());
            return nullptr;
        }

    #endif

    return handle;
}

void * _dlsym(ModuleHandle handle, const std::string& symbol)
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

bool LoadModule(const std::string& name, Version minVersion /*= Version()*/)
{
    DuskLogLoad("Loading module '%s'", name);

    ModuleHandle handle = _dlopen(name);
    if (!handle) {
        return false;
    }

    DuskModuleDefinition * def = static_cast<DuskModuleDefinition *>(_dlsym(handle, "_DuskModule"));
    if (!def) {
        DuskLogError("Failed to find _DuskModule symbol");
        return false;
    }
    
    if (def->GetVersion) {
        Version ver = def->GetVersion();
        if (minVersion > ver) {
            _dlclose(handle);

            DuskLogError("Failed to load module '%s', found version %s but minimum version is %s", 
                name, ver.GetString(), minVersion.GetString());
            return false;
        }

        DuskLogVerbose("Found module '%s' version %s", name, ver.GetString());
    }

    if (def->Initialize) {
        if (!def->Initialize()) {
            _dlclose(handle);

            DuskLogError("Failed to initialize module '%s'", name);
            return false;
        }
    }

    _Modules.emplace(name, handle);

    return true;
}

DUSK_ENGINE_API
void FreeModule(const std::string& name)
{
    auto it = _Modules.find(name);
    if (it == _Modules.end()) {
        DuskLogWarn("Failed to free module '%s', module is not loaded", name);
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
