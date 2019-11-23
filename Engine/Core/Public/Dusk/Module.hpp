#ifndef DUSK_MODULE_HPP
#define DUSK_MODULE_HPP

#include <string>

namespace Dusk {

struct DuskModule {
    const char * Name;
    void (*Init)();
    void (*Term)();
};

bool LoadModule(const std::string& name);

void FreeModules();

#define DUSK_MODULE() \
    extern "C" struct DuskModule _DuskModule = 

}

#endif // DUSK_MODULE_HPP