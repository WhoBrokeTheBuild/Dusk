#ifndef DUSK_MODULE_HPP
#define DUSK_MODULE_HPP

#include <Dusk/Config.hpp>

#include <string>
#include <initializer_list>

namespace Dusk {

struct DUSK_CORE_API DuskModule {
    const char * Name;
    void (*Init)();
    void (*Term)();
};

DUSK_CORE_API
bool LoadModule(const std::string& name);

DUSK_CORE_API
bool LoadModuleArray(const std::initializer_list<std::string>& names);

DUSK_CORE_API
void FreeModules();

#define DUSK_MODULE() \
    extern "C" DUSK_API_EXPORT struct DuskModule _DuskModule = 

}

#endif // DUSK_MODULE_HPP
