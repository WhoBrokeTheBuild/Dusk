#include <Dusk/Module.hpp>

#include <Dusk/Log.hpp>

using namespace Dusk;

void FreeTypeModuleInit()
{
}

void FreeTypeModuleTerm()
{
}

DEFINE_MODULE() {
    "FreeType",
    FreeTypeModuleInit,
    FreeTypeModuleTerm,
};
