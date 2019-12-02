#include <Dusk/Module.hpp>

#include <vector>

#if defined(DUSK_OS_WINDOWS)
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

namespace Dusk {
    
#if defined(DUSK_OS_WINDOWS)
    typedef HMODULE ModuleHandle;
#else
    typedef void * ModuleHandle;
#endif // __linux__

std::vector<ModuleHandle> _Modules;

bool LoadModule(const std::string& name) {
    ModuleHandle module = nullptr;

    printf("Loading %s\n", name.c_str());

    #if defined(DUSK_OS_WINDOWS)
        module = LoadLibraryA(name.c_str());
        if (!module) {
            fprintf(stderr, "Failed to load %s\n", name.c_str());
            return false;
        }
        
        DuskModule * def = (DuskModule *)GetProcAddress(module, "_DuskModule");
    #else
        module = dlopen(name.c_str(), RTLD_GLOBAL | RTLD_NOW);
        if (!module) {
            fprintf(stderr, "Failed to load %s, %s\n", name.c_str(), dlerror());
            return false;
        }

        DuskModule * def = (DuskModule *)dlsym(module, "_DuskModule");
    #endif

    if (!def) {
        fprintf(stderr, "Failed to find _DuskModule symbol\n");
        return false;
    }

    _Modules.push_back(module);

    if (def->Init) {
        def->Init();
    }

    return true;
}

void FreeModules() {
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
