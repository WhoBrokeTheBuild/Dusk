#ifndef DUSK_VULKAN_MATERIAL_HPP
#define DUSK_VULKAN_MATERIAL_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>
#include <Dusk/Material.hpp>

namespace Dusk::Vulkan {

class DUSK_VULKAN_API VulkanMaterial : public Material
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanMaterial)

    VulkanMaterial() = default;

    virtual ~VulkanMaterial() = default;

    // bool Initialize() override;

    // void Terminate() override;

    bool Create();

    void Destroy();

    void GenerateCommands(VkCommandBuffer vkCommandBuffer);

private:

    VkDescriptorSet _vkDescriptorSet;

}; // class VulkanMaterial

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_MATERIAL_HPP