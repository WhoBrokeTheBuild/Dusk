#include <Dusk/Module.hpp>

namespace Dusk::DirectX {

void ModuleInit() {
}

void ModuleTerm() {

}

DUSK_MODULE() {
    .Name = "DirectX",
    .Init = ModuleInit,
    .Term = ModuleTerm,
};

}