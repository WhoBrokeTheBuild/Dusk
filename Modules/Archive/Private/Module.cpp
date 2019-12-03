#include <Dusk/Module.hpp>

namespace Dusk::Archive {

void ModuleInit() {
}

void ModuleTerm() {

}

DUSK_MODULE() {
    .Name = "Archive",
    .Init = ModuleInit,
    .Term = ModuleTerm,
};

}