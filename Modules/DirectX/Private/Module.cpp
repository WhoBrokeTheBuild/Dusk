#include <Dusk/Module.hpp>

#include <Dusk/DirectX/GraphicsDriver.hpp>

namespace Dusk::DirectX {

void ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<Dusk::GraphicsDriver>(New GraphicsDriver()));
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
}

DEFINE_MODULE() {
    "DirectX",
    ModuleInit,
    ModuleTerm,
};

} // namespace Dusk::DirectX