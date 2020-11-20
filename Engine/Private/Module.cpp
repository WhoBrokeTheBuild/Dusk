#include <Dusk/Module.hpp>

#include <Dusk/Log.hpp>

#if defined(DUSK_OS_WINDOWS)
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

#include <vector>

namespace Dusk {
    
#if defined(DUSK_OS_WINDOWS)
    typedef HMODULE ModuleHandle;
#else
    typedef void * ModuleHandle;
#endif // __linux__

std::vector<ModuleHandle> _Modules;

bool LoadModule(const std::string& name)
{
    ModuleHandle module = nullptr;

    DuskLogLoad("Loading '%s'", name);

    #if defined(DUSK_OS_WINDOWS)
        DuskLogVerbose("Loading Module from PATH: '%s'", getenv("PATH"));

        module = LoadLibraryA(name.c_str());
        if (!module) {
            char message[256];
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, GetLastError(), GetUserDefaultUILanguage(), message, sizeof(message), NULL);

            DuskLogError("Failed to load '%s', %s", name, message);
            return false;
        }
        
        DuskModule * def = (DuskModule *)GetProcAddress(module, "_DuskModule");
    #else
        // DuskLogVerbose("Loading Module from LD_LIBRARY_PATH: '%s'", getenv("LD_LIBRARY_PATH"));

        std::string filename = "lib" + name + ".so";
        module = dlopen(filename.c_str(), RTLD_GLOBAL | RTLD_NOW);
        if (!module) {
            DuskLogError("Failed to load '%s', %s", filename, dlerror());
            return false;
        }

        DuskModule * def = (DuskModule *)dlsym(module, "_DuskModule");
    #endif

    if (!def) {
        DuskLogError("Failed to find _DuskModule symbol");
        return false;
    }

    _Modules.push_back(module);

    if (def->Init) {
        def->Init();
    }

    return true;
}

bool LoadModuleArray(const std::initializer_list<std::string>& names)
{
    for (const auto& name : names) {
        if (!LoadModule(name)) {
            return false;
        }
    }
    return true;
}

void FreeModules()
{
    for (auto module : _Modules) {  
        #if defined(DUSK_OS_WINDOWS)
            DuskModule * def = (DuskModule *)GetProcAddress(module, "_DuskModule");
        #else
            DuskModule * def = (DuskModule *)dlsym(module, "_DuskModule");
        #endif
        
        if (def && def->Term) {
            def->Term();
        }

        #if defined(DUSK_OS_WINDOWS)
            FreeLibrary(module);
        #else
            dlclose(module);
        #endif
    }
    _Modules.clear();
}

}
