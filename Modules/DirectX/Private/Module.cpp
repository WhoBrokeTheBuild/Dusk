#include <Dusk/Module.hpp>

namespace Dusk::DirectX {

void ModuleInit() {
}

void ModuleTerm() {

}

DUSK_MODULE() {
    "DirectX",
    ModuleInit,
    ModuleTerm,
};

}