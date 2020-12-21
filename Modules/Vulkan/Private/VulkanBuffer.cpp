#include <Dusk/Vulkan/VulkanBuffer.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
bool VulkanBuffer::Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage)
{
    bool result;
    VkResult vkResult;

    _bufferUsage = bufferUsage;
    _memoryUsage = memoryUsage;
    _size = static_cast<VkDeviceSize>(size);

    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    auto vkBufferUsage = GetVkBufferUsage(_bufferUsage);
    auto vkMemoryUsage = GetVkMemoryUsage(_memoryUsage);

    if (!vkBufferUsage || !vkMemoryUsage) {
        return false;
    }

    // If we are uploading to a GPU only buffer, use a staging buffer
    if (_memoryUsage == MemoryUsage::GPU) {
        VkBuffer stagingBuffer;
        VmaAllocation stagingAllocation;

        result = gfx->CreateBuffer(&stagingBuffer, &stagingAllocation, _size, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VMA_MEMORY_USAGE_CPU_ONLY);

        if (!result) {
            DuskLogError("CreateBuffer() failed, unable to create staging buffer");
            return false;
        }

        void * ptr;
        vkResult = vmaMapMemory(gfx->GetAllocator(), stagingAllocation, &ptr);
        if (vkResult != VK_SUCCESS) {
            DuskLogError("vmaMapMemory() failed");
            return false;
        }

        memcpy(ptr, data, _size);

        vmaUnmapMemory(gfx->GetAllocator(), stagingAllocation);

        result = gfx->CreateBuffer(&_vkBuffer, &_vmaAllocation, _size, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | vkBufferUsage.value(),
            VMA_MEMORY_USAGE_GPU_ONLY);
            
        if (!result) {
            DuskLogError("CreateBuffer() failed, unable to create buffer");
            return false;
        }

        result = gfx->CopyBuffer(stagingBuffer, _vkBuffer, _size);

        if (!result) {
            DuskLogError("CopyBuffer() failed, unable to copy staging buffer to buffer");
        }

        vkDestroyBuffer(gfx->GetDevice(), stagingBuffer, nullptr);
        vmaFreeMemory(gfx->GetAllocator(), stagingAllocation);
    }
    else if (_memoryUsage == MemoryUsage::UploadOnce || 
             _memoryUsage == MemoryUsage::UploadOften) {
                 
        result = gfx->CreateBuffer(&_vkBuffer, &_vmaAllocation, _size, 
            vkBufferUsage.value(),
            vkMemoryUsage.value());
            
        if (!result) {
            DuskLogError("CreateBuffer() failed, unable to create buffer");
            return false;
        }

        void * ptr;
        vmaMapMemory(gfx->GetAllocator(), _vmaAllocation, &ptr);
        memcpy(ptr, data, _size);
        vmaUnmapMemory(gfx->GetAllocator(), _vmaAllocation);
    }
    else {
        DuskLogError("MemoryUsage::Download is not yet supported");
        return false;
    }

    return true;
}

DUSK_VULKAN_API
void VulkanBuffer::Terminate()
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    vkDestroyBuffer(gfx->GetDevice(), _vkBuffer, nullptr);
    vmaFreeMemory(gfx->GetAllocator(), _vmaAllocation);
}

} // namespace Dusk::Vulkan