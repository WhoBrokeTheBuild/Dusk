#include <Dusk/Module.hpp>

namespace Dusk::Vulkan {

void ModuleInit() {
}

void ModuleTerm() {

}

DUSK_MODULE() {
    .Name = "Vulkan",
    .Init = ModuleInit,
    .Term = ModuleTerm,
};

}