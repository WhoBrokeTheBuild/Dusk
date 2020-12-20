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

} // namespace Dusk::Vulkan