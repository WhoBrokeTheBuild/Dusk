#include <Dusk/Module.hpp>

#include <Dusk/DirectX/GraphicsDriver.hpp>

void DirectXModuleInit()
{
    SetGraphicsDriver(std::unique_ptr<Dusk::GraphicsDriver>(New GraphicsDriver()));
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
