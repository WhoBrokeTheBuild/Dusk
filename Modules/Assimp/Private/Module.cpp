#include <Dusk/Module.hpp>

namespace Dusk::Assimp {

void ModuleInit() {
}

void ModuleTerm() {

}

DUSK_MODULE() {
    .Name = "Assimp",
    .Init = ModuleInit,
    .Term = ModuleTerm,
};

}