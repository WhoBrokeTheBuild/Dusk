#include <Dusk/Module.hpp>

#include <Dusk/Vulkan/GraphicsDriver.hpp>

namespace Dusk::Vulkan {

void ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<IGraphicsDriver>(new GraphicsDriver()));
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