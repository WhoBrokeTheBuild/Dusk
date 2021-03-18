#include <Dusk/Vulkan/VulkanPrimitive.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>
#include <Dusk/Vulkan/VulkanBuffer.hpp>
#include <Dusk/Benchmark.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
void VulkanPrimitive::Terminate()
{
    if (_vertexBuffer) {
        _vertexBuffer->Terminate();
    }

    if (_indexBuffer) {
        _indexBuffer->Terminate();
    }
}

DUSK_VULKAN_API
bool VulkanPrimitive::Load(const std::unique_ptr<PrimitiveData>& data)
{
    DuskBenchmarkStart();

    auto gfx = VulkanGraphicsDriver::GetInstance();

    bool result;

    _material = data->GetMaterial();
    if (_material) {
        _material = gfx->GetDefaultMaterial();
    }

    const auto& indexList = data->GetIndexList();
    const auto& vertexList = data->GetVertexList();

    // Check if the buffer has been created, if not create it
    if (!_vertexBuffer) {
        _vertexBuffer.reset(New VulkanBuffer());
    }

    result = _vertexBuffer->Initialize(
        vertexList.size() * sizeof(Vertex),
        reinterpret_cast<uint8_t *>(vertexList.data()),
        BufferUsage::Vertex, MemoryUsage::GPU);

    if (!result) {
        LogError(DUSK_ANCHOR, "Failed to upload vertex list to GPU buffer");
        return false;
    }

    if (indexList.empty()) {
        _indexed = false;
        _count = vertexList.size();
    }
    else {
        LogFatal(DUSK_ANCHOR, "UNTESTED");

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

    DuskBenchmarkEnd();
    return true;
}

DUSK_VULKAN_API
void VulkanPrimitive::GenerateCommands(VkCommandBuffer vkCommandBuffer)
{
    // TODO: Test moving index buffer inside of other if _indexed check
    if (_indexed) {
        vkCmdBindIndexBuffer(vkCommandBuffer, _indexBuffer->GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }

    VkBuffer vertexBuffers[] = { _vertexBuffer->GetVkBuffer() };
    VkDeviceSize offsets[] = { 0 };
    
    vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vertexBuffers, offsets);
    
    if (_indexed) {
        vkCmdDrawIndexed(vkCommandBuffer, _count, 1, 0, 0, 0);
    }
    else {
        vkCmdDraw(vkCommandBuffer, _count, 1, 0, 0);
    }
}

} // namespace Dusk::Vulkan
