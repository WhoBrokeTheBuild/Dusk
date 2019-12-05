#include <Dusk/Module.hpp>

#include <Dusk/Vulkan/Drivers/GraphicsDriver.hpp>

namespace Dusk::Vulkan {

void ModuleInit() {
    SetGraphicsDriver(new GraphicsDriver());
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
}

DUSK_MODULE() {
    "Vulkan",
    ModuleInit,
    ModuleTerm,
};

}