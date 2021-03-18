#include <Dusk/Module.hpp>

#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk::Vulkan {

static std::unique_ptr<VulkanGraphicsDriver> _VulkanGraphicsDriver;

bool ModuleInit()
{
    try {
        _VulkanGraphicsDriver.reset(new(__LINE__, __FILE__) VulkanGraphicsDriver());
        _VulkanGraphicsDriver->Initialize(); // TODO: Remove
    }
    catch (const std::exception& e) {
        LogError(DUSK_ANCHOR, "{}", e.what());
        return false;
    }

    return true;
}

void ModuleTerm()
{
    _VulkanGraphicsDriver->Terminate();
    _VulkanGraphicsDriver.reset();
}

Version GetVersion()
{
    return Version(DUSK_VERSION_MAJOR, DUSK_VERSION_MINOR, DUSK_VERSION_PATCH);
}

DUSK_DEFINE_MODULE {
    .Name       = "DuskVulkan",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
    .GetVersion = GetVersion,
};

} // namespace Dusk::Vulkan
