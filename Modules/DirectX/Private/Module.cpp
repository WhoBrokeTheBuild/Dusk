#include <Dusk/Module.hpp>

namespace Dusk::DirectX {

void ModuleInit() {
}

void ModuleTerm() {

}

DEFINE_MODULE() {
    "DirectX",
    ModuleInit,
    ModuleTerm,
};

}