#include <Dusk/Module.hpp>

namespace Dusk::FreeType {

bool ModuleInit()
{
    return true;
}

void ModuleTerm()
{
}

Version GetVersion()
{
    return Version(DUSK_VERSION_MAJOR, DUSK_VERSION_MINOR, DUSK_VERSION_PATCH);
}

DUSK_DEFINE_MODULE {
    .Name       = "DuskFreeType",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
    .GetVersion = GetVersion,
};

} // namespace Dusk::FreeType