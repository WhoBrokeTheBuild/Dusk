#include <Dusk/Module.hpp>

#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

using namespace Dusk;

void VulkanModuleInit()
{
    SetGraphicsDriver(std::unique_ptr<GraphicsDriver>(New VulkanGraphicsDriver()));
}

void VulkanModuleTerm()
{
    SetGraphicsDriver(nullptr);
}

DEFINE_MODULE() {
    "Vulkan",
    VulkanModuleInit,
    VulkanModuleTerm,
};
