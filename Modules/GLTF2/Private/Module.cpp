#include <Dusk/Module.hpp>

namespace Dusk::GLTF2 {

void ModuleInit() {
}

void ModuleTerm() {

}

DUSK_MODULE() {
    .Name = "GLTF2",
    .Init = ModuleInit,
    .Term = ModuleTerm,
};

}