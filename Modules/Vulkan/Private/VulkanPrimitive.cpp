#include <Dusk/Vulkan/VulkanPrimitive.hpp>

#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
VulkanPrimitive::~VulkanPrimitive()
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());
}

bool VulkanPrimitive::Load(const std::unique_ptr<PrimitiveData>& data)
{
    bool result;

    const auto& indexList = data->GetIndexList();
    const auto& vertexList = data->GetVertexList();

    _vertexBuffer.reset(New VulkanBuffer());

    result = _vertexBuffer->Initialize(
        vertexList.size() * sizeof(Vertex),
        reinterpret_cast<uint8_t *>(vertexList.data()),
        BufferUsage::Vertex, MemoryUsage::GPU);

    if (!result) {
        DuskLogError("Failed to upload vertex list to GPU buffer");
        return false;
    }

    if (indexList.empty()) {
        _indexed = false;
        _count = vertexList.size();
    }
    else {
        DuskLogFatal("UNTESTED");

        _indexed = true;
        _count = indexList.size();

        _indexBuffer.reset(New VulkanBuffer());

        result = _indexBuffer->Initialize(
            indexList.size() * sizeof(uint32_t), 
            reinterpret_cast<uint8_t *>(indexList.data()), 
            BufferUsage::Index, MemoryUsage::GPU);
        
        if (!result) {
            return false;
        }
    }

    return true;
}

void VulkanPrimitive::GenerateCommands(VkCommandBuffer vkCommandBuffer)
{
    // TODO: Test moving index buffer inside of other if _indexed check
    if (_indexed) {
        vkCmdBindIndexBuffer(vkCommandBuffer, _indexBuffer->GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }

    VkBuffer vertexBuffers[] = { _vertexBuffer->GetVkBuffer() };
    VkDeviceSize offsets[] = { 0 };
    
    vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vertexBuffers, offsets);
    

    {
        VkDebugUtilsLabelEXT label = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pNext = NULL,
            .pLabelName = "ActualDraw",
            .color = { 0.4f, 0.3f, 0.2f, 0.1f },
        };
        vkCmdBeginDebugUtilsLabelEXT(vkCommandBuffer, &label);
    }


    if (_indexed) {
        vkCmdDrawIndexed(vkCommandBuffer, _count, 1, 0, 0, 0);
    }
    else {
        vkCmdDraw(vkCommandBuffer, _count, 1, 0, 0);
    }

    vkCmdEndDebugUtilsLabelEXT(vkCommandBuffer);
}

} // namespace Dusk::Vulkan
