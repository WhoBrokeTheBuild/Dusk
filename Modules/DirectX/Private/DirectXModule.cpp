#include <Dusk/Module.hpp>

#include <Dusk/DirectX/DirectXGraphicsDriver.hpp>

namespace Dusk::DirectX {

static std::unique_ptr<DirectXGraphicsDriver> _DirectXGraphicsDriver;

bool ModuleInit()
{
    try {
        _DirectXGraphicsDriver.reset(new(__LINE__, __FILE__) DirectXGraphicsDriver());
        _DirectXGraphicsDriver->Initialize(); // TODO: Remove
    }
    catch (const std::exception& e) {
        LogError(DUSK_ANCHOR, "{}", e);
        return false;
    }
    
    return true;
}

void ModuleTerm()
{
    _DirectXGraphicsDriver.reset();
}

Version GetVersion()
{
    return Version(DUSK_VERSION_MAJOR, DUSK_VERSION_MINOR, DUSK_VERSION_PATCH);
}

DUSK_DEFINE_MODULE {
    .Name       = "DirectX",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
    .GetVersion = GetVersion,
};

} // namespace Dusk::DirectX