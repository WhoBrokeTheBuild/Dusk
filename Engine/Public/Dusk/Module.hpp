#ifndef DEFINE_MODULE_HPP
#define DEFINE_MODULE_HPP

#include <Dusk/Config.hpp>

#include <string>
#include <initializer_list>

namespace Dusk {

struct DUSK_ENGINE_API DuskModule {
    const char * Name;
    void (*Init)();
    void (*Term)();
};

DUSK_ENGINE_API
bool LoadModule(const std::string& name);

DUSK_ENGINE_API
bool LoadModuleArray(const std::initializer_list<std::string>& names);

DUSK_ENGINE_API
void FreeModules();

#define DEFINE_MODULE() \
    extern "C" DUSK_API_EXPORT struct DuskModule _DuskModule = 

}

#endif // DEFINE_MODULE_HPP
