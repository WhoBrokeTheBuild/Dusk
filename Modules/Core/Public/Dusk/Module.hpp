#ifndef DUSK_MODULE_HPP
#define DUSK_MODULE_HPP

#include <Dusk/Core.hpp>

#include <string>

namespace Dusk {

struct DUSK_CORE_API DuskModule {
    const char * Name;
    void (*Init)();
    void (*Term)();
};

DUSK_CORE_API bool LoadModule(const std::string& name);

DUSK_CORE_API void FreeModules();

#define DUSK_MODULE() \
    extern "C" DUSK_API_EXPORT struct DuskModule _DuskModule = 

}

#endif // DUSK_MODULE_HPP
