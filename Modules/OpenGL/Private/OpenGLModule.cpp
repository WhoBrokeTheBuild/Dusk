#include <Dusk/Module.hpp>

#include <Dusk/OpenGL/OpenGLGraphicsDriver.hpp>

namespace Dusk::OpenGL {

bool ModuleInit()
{
    SetGraphicsDriver(std::unique_ptr<GraphicsDriver>(New OpenGLGraphicsDriver()));
    if (!GetGraphicsDriver()->Initialize()) {
        SetGraphicsDriver(nullptr);
        return false;
    }

    return true;
}

void ModuleTerm()
{
    SetGraphicsDriver(nullptr);
}

Version GetVersion()
{
    return Version(DUSK_VERSION_MAJOR, DUSK_VERSION_MINOR, DUSK_VERSION_PATCH);
}

DUSK_DEFINE_MODULE {
    .Name       = "DuskOpenGL",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
    .GetVersion = GetVersion,
};

} // namespace Dusk::OpenGL