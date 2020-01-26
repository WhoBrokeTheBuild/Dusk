#include <Dusk/Module.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/OpenGL/GraphicsDriver.hpp>
#include <Dusk/OpenGL/InputDriver.hpp>

namespace Dusk::OpenGL {

void ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<Dusk::GraphicsDriver>(new GraphicsDriver()));
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
}

DUSK_MODULE() {
    "OpenGL",
    ModuleInit,
    ModuleTerm,
};

} // namespace Dusk::OpenGL