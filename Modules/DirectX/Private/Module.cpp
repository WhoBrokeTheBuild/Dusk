#include <Dusk/Module.hpp>

#include <Dusk/DirectX/GraphicsDriver.hpp>

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
