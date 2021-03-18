#include <Dusk/Module.hpp>

#include <Dusk/OpenGL/OpenGLGraphicsDriver.hpp>

namespace Dusk::OpenGL {

static std::unique_ptr<OpenGLGraphicsDriver> _OpenGLGraphicsDriver;

bool ModuleInit()
{
    try {
        _OpenGLGraphicsDriver.reset(new(__LINE__, __FILE__) OpenGLGraphicsDriver());
        _OpenGLGraphicsDriver->Initialize(); // TODO: Remove
    }
    catch (const std::exception& e) {
        LogError(DUSK_ANCHOR, "{}", e.what());
        return false;
    }

    return true;
}

void ModuleTerm()
{
    _OpenGLGraphicsDriver->Terminate();
    _OpenGLGraphicsDriver.reset();
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