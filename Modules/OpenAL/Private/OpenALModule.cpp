#include <Dusk/Module.hpp>

namespace Dusk::OpenAL {

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
    .Name       = "DuskOpenAL",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
    .GetVersion = GetVersion,
};

} // namespace Dusk::OpenAL
