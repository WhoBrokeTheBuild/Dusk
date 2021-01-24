#include <Dusk/Vulkan/VulkanMesh.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
VulkanMesh::~VulkanMesh()
{
}

DUSK_VULKAN_API
bool VulkanMesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
    return true;
}

DUSK_VULKAN_API
void VulkanMesh::GenerateCommands(VkCommandBuffer vkCommandBuffer)
{
    VulkanPipeline * vkPipeline = DUSK_VULKAN_PIPELINE(_pipeline.get());
    vkPipeline->GenerateCommands(vkCommandBuffer);

    for (auto& primitive : _primitiveList) {
        VulkanPrimitive * vkPrimitive = DUSK_VULKAN_PRIMITIVE(primitive.get());
        vkPrimitive->GenerateCommands(vkCommandBuffer);
    }
}

} // namespace Dusk::Vulkan