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
void VulkanMesh::Render()
{

}

DUSK_VULKAN_API
bool VulkanMesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
    return true;
}

// void VulkanMesh::GenerateBindCommands(VkCommandBuffer vkCommandBuffer)
// {
//     // Must be >= the size of _vkBuffers
//     VkDeviceSize offsets[] = { 0, 0, 0, 0, 0, 0, 0 };

//     vkCmdBindVertexBuffers(vkCommandBuffer, 0, static_cast<uint32_t>(_vkBuffers.size()), _vkBuffers.data(), offsets);
// }

// void VulkanMesh::GenerateDrawCommands(VkCommandBuffer vkCommandBuffer)
// {
//     if (_indexed) {
//         vkCmdDrawIndexed(vkCommandBuffer, _count, 1, 0, 0, 0);
//     }
//     else {
//         vkCmdDraw(vkCommandBuffer, _count, 1, 0, 0);
//     }
// }

} // namespace Dusk::Vulkan