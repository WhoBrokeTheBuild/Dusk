#include <Dusk/Module.hpp>

namespace Dusk::GLTF2 {

void ModuleInit() {
}

void ModuleTerm() {

}

DUSK_MODULE() {
    "GLTF2",
    ModuleInit,
    ModuleTerm,
};

}