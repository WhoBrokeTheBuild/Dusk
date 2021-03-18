#include <Dusk/Vulkan/VulkanMaterial.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
bool VulkanMaterial::Create()
{
    auto gfx = VulkanGraphicsDriver::GetInstance();
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
