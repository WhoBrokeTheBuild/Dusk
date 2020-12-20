#include <Dusk/Vulkan/VulkanBuffer.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk::Vulkan {

bool VulkanBuffer::Initialize(size_t size, uint8_t * data, BufferUsage usage)
{
    bool result;
    VkResult vkResult;

    _size = static_cast<VkDeviceSize>(size);

    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    auto vkUsage = GetVkBufferUsage(usage);
    if (!vkUsage) {
        return false;
    }

    VkBuffer stagingBuffer;
    VmaAllocation stagingAllocation;

    result = gfx->CreateBuffer(&stagingBuffer, &stagingAllocation, _size, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_CPU_ONLY);

    if (!result) {
        DuskLogError("Failed to create staging buffer");
        return false;
    }

    void * ptr;
    vmaMapMemory(gfx->GetAllocator(), stagingAllocation, &ptr);
    memcpy(ptr, data, size);
    vmaUnmapMemory(gfx->GetAllocator(), stagingAllocation);

    result = gfx->CreateBuffer(&_vkBuffer, &_vmaAllocation, _size, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | vkUsage.value(),
        VMA_MEMORY_USAGE_GPU_ONLY);
        
    if (!result) {
        DuskLogError("Failed to create buffer");
        return false;
    }

    result = gfx->CopyBuffer(stagingBuffer, _vkBuffer, _size);

    if (!result) {
        DuskLogError("Failed to copy staging buffer to buffer");
    }

    vkDestroyBuffer(gfx->GetDevice(), stagingBuffer, nullptr);
    vmaFreeMemory(gfx->GetAllocator(), stagingAllocation);

    return true;
}

void VulkanBuffer::Terminate()
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    vkDestroyBuffer(gfx->GetDevice(), _vkBuffer, nullptr);
    vmaFreeMemory(gfx->GetAllocator(), _vmaAllocation);
}

} // namespace Dusk::Vulkan