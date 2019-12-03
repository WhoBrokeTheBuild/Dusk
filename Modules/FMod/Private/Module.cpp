#include <Dusk/Module.hpp>

namespace Dusk::FMod {

void ModuleInit() {
}

void ModuleTerm() {

}

DUSK_MODULE() {
    .Name = "FMod",
    .Init = ModuleInit,
    .Term = ModuleTerm,
};

}