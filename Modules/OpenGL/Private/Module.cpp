#include <Dusk/Module.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/OpenGL/Drivers/GraphicsDriver.hpp>

namespace Dusk::OpenGL {

void ModuleInit() {
    SetGraphicsDriver(new GraphicsDriver());
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