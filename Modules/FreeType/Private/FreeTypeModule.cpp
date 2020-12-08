#include <Dusk/Module.hpp>

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
