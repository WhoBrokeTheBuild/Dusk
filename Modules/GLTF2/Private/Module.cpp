#include <Dusk/Module.hpp>

namespace Dusk::GLTF2 {

void ModuleInit() {
}

void ModuleTerm() {

}

DEFINE_MODULE() {
    "GLTF2",
    ModuleInit,
    ModuleTerm,
};

}