#include <Dusk/Module.hpp>

#include <Dusk/OpenGL/GraphicsDriver.hpp>
#include <Dusk/OpenGL/InputDriver.hpp>

namespace Dusk::OpenGL {

void ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<Dusk::GraphicsDriver>(New GraphicsDriver()));
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
}

DEFINE_MODULE() {
    "OpenGL",
    ModuleInit,
    ModuleTerm,
};

} // namespace Dusk::OpenGL