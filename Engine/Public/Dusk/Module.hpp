#ifndef DUSK_MODULE_HPP
#define DUSK_MODULE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Version.hpp>

#include <string>
#include <initializer_list>

namespace Dusk {

struct DUSK_ENGINE_API DuskModuleDefinition
{
    const char * Name;
    bool (*Initialize)();
    void (*Terminate)();
    Version (*GetVersion)();
};

DUSK_ENGINE_API
bool LoadModule(const std::string& name, Version minVersion = Version());

DUSK_ENGINE_API
void FreeModule(const std::string& name);

DUSK_ENGINE_API
void FreeAllModules();

#define DUSK_DEFINE_MODULE \
    extern "C" DUSK_API_EXPORT struct Dusk::DuskModuleDefinition _DuskModule = 

}

#endif // DUSK_MODULE_HPP
