#ifndef DUSK_MODULE_HPP
#define DUSK_MODULE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Version.hpp>
#include <Dusk/String.hpp>

#include <initializer_list>

namespace Dusk {

struct DUSK_CORE_API DuskModuleDefinition
{
    const char * Name;
    bool (*Initialize)();
    void (*Terminate)();
    Version (*GetVersion)();
};

DUSK_CORE_API
bool LoadModule(const string& name, Version minVersion = Version());

DUSK_CORE_API
void FreeModule(const string& name);

DUSK_CORE_API
void FreeAllModules();

#define DUSK_DEFINE_MODULE \
    extern "C" DUSK_API_EXPORT struct Dusk::DuskModuleDefinition _DuskModule = 

}

#endif // DUSK_MODULE_HPP
