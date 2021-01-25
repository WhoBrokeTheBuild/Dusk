#include <Dusk/Vulkan/VulkanMesh.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
bool VulkanMesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
    for (const auto& primitiveData : data) {
        std::unique_ptr<Primitive> primitive = std::unique_ptr<Primitive>(New VulkanPrimitive());
        if (!primitive->Load(primitiveData)) {
            return false;
        }

        _primitiveList.push_back(std::move(primitive));
    }

    return true;
}

DUSK_VULKAN_API
void VulkanMesh::Render(RenderContext * ctx)
{
    VulkanRenderContext * vkRenderContext = DUSK_VULKAN_RENDER_CONTEXT(ctx);
    VkCommandBuffer vkCommandBuffer = vkRenderContext->GetVkCommandBuffer();

    // VulkanPipeline * vkPipeline = DUSK_VULKAN_PIPELINE(_pipeline.get());
    // vkPipeline->GenerateCommands(vkCommandBuffer);

    for (auto& primitive : _primitiveList) {
        VulkanPrimitive * vkPrimitive = DUSK_VULKAN_PRIMITIVE(primitive.get());
        vkPrimitive->GenerateCommands(vkCommandBuffer);
    }
}

} // namespace Dusk::Vulkan