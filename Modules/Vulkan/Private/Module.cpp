#include <Dusk/Module.hpp>

#include <Dusk/Vulkan/GraphicsDriver.hpp>

namespace Dusk::Vulkan {

void ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<Dusk::GraphicsDriver>(New GraphicsDriver()));
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
}

DEFINE_MODULE() {
    "Vulkan",
    ModuleInit,
    ModuleTerm,
};

}