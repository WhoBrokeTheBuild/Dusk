#include <Dusk/Vulkan/VulkanMaterial.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
bool VulkanMaterial::Create()
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());
    if (!gfx->CreateDescriptorSet(&_vkDescriptorSet)) {
        return false;
    }

}

DUSK_VULKAN_API
void VulkanMaterial::Destroy()
{

}

DUSK_VULKAN_API
void VulkanMaterial::GenerateCommands(VkCommandBuffer vkCommandBuffer)
{

}

} // namespace Dusk::Vulkan
