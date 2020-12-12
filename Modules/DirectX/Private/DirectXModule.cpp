#include <Dusk/Module.hpp>

#include <Dusk/DirectX/DirectXGraphicsDriver.hpp>

using namespace Dusk;

void DirectXModuleInit()
{
    SetGraphicsDriver(std::unique_ptr<GraphicsDriver>(New DirectXGraphicsDriver()));
    if (!GetGraphicsDriver()->Initialize()) {
        SetGraphicsDriver(nullptr);
        return false;
    }
}

void DirectXModuleTerm()
{
    SetGraphicsDriver(nullptr);
}

DUSK_DEFINE_MODULE {
    "DirectX",
    DirectXModuleInit,
    DirectXModuleTerm,
};
