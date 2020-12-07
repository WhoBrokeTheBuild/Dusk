#include <Dusk/Module.hpp>

using namespace Dusk;

void ArchiveModuleInit()
{

}

void ArchiveModuleTerm()
{

}

DEFINE_MODULE() {
    "Archive",
    ArchiveModuleInit,
    ArchiveModuleTerm,
};
