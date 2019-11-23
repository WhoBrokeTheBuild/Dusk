#include <Dusk/Module.hpp>

#include <dlfcn.h>

#include <vector>

namespace Dusk {

std::vector<void *> _Modules;

bool LoadModule(const std::string& name) {
    void * module = nullptr;

    std::string filename = "../../Build/Engine/" + name + "/libDusk" + name + ".so";

    module = dlopen(filename.c_str(), RTLD_GLOBAL | RTLD_NOW);
    if (!module) {
        fprintf(stderr, "Failed to load %s, %s\n", filename.c_str(), dlerror());
        return false;
    }

    _Modules.push_back(module);

    DuskModule * def = (DuskModule *)dlsym(module, "_DuskModule");

    if (!def) {
        fprintf(stderr, "Failed to find _DuskModule symbol\n");
        return false;
    }

    if (def->Init) {
        def->Init();
    }

    return true;
}

void FreeModules() {
    for (void * module : _Modules) {
        DuskModule * def = (DuskModule *)dlsym(module, "_DuskModule");
        if (def && def->Term) {
            def->Term();
        }

        dlclose(module);
    }
    _Modules.clear();
}

}