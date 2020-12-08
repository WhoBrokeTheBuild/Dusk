#include <Dusk/Module.hpp>

#include <Dusk/DirectX/DirectXGraphicsDriver.hpp>

using namespace Dusk;

void DirectXModuleInit()
{
    SetGraphicsDriver(std::unique_ptr<GraphicsDriver>(New DirectXGraphicsDriver()));
}

void DirectXModuleTerm()
{
    SetGraphicsDriver(nullptr);
}

DEFINE_MODULE() {
    "DirectX",
    DirectXModuleInit,
    DirectXModuleTerm,
};
