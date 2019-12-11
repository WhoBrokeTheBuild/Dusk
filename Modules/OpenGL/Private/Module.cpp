#include <Dusk/Module.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/OpenGL/GraphicsDriver.hpp>
#include <Dusk/OpenGL/ShaderImporter.hpp>

namespace Dusk::OpenGL {

void ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<IGraphicsDriver>(new GraphicsDriver()));
    // SetShaderImporter(new ShaderImporter());
}

void ModuleTerm() {
    // SetShaderImporter(nullptr);
    SetGraphicsDriver(nullptr);
}

DUSK_MODULE() {
    "OpenGL",
    ModuleInit,
    ModuleTerm,
};

} // namespace Dusk::OpenGL