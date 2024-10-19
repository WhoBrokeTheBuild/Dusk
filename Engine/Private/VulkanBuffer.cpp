#include <Dusk/VulkanBuffer.hpp>
#include <Dusk/Graphics.hpp>

namespace dusk {

DUSK_API
VulkanBuffer::~VulkanBuffer()
{
    Destroy();
}

DUSK_API
void VulkanBuffer::Create(VkBufferUsageFlagBits bufferUsage, VmaMemoryUsage memoryUsage, VkDeviceSize size, void * data /* = nullptr */)
{
    VkResult result;

    Destroy();

    _size = size;
    _bufferUsage = bufferUsage;
    _memoryUsage = memoryUsage;

    if (_memoryUsage == VMA_MEMORY_USAGE_GPU_ONLY) {
        if (not data) {
            throw Exception("Attempting to create a GPU only buffer with no data");
        }

        auto stagingBufferCreateInfo = VkBufferCreateInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = _size,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        auto stagingAllocationCreateInfo = VmaAllocationCreateInfo{
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_CPU_ONLY,
        };

        VkBuffer stagingBuffer;
        VmaAllocation stagingAllocation;
        VmaAllocationInfo stagingAllocationInfo;
    
        result = vmaCreateBuffer(
            Graphics::Allocator,
            &stagingBufferCreateInfo,
            &stagingAllocationCreateInfo,
            &stagingBuffer,
            &stagingAllocation,
            &stagingAllocationInfo
        );
        CheckVkResult("vmaCreateBuffer", result);

        memcpy(stagingAllocationInfo.pMappedData, data, _size);

        auto bufferCreateInfo = VkBufferCreateInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = _size,
            .usage = VkBufferUsageFlags(VK_BUFFER_USAGE_TRANSFER_DST_BIT | _bufferUsage),
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        auto allocationCreateInfo = VmaAllocationCreateInfo{
            .usage = _memoryUsage,
        };

        result = vmaCreateBuffer(
            Graphics::Allocator,
            &bufferCreateInfo,
            &allocationCreateInfo,
            &_buffer,
            &_allocation,
            nullptr
        );
        CheckVkResult("vmaCreateBuffer", result);

        auto region = VkBufferCopy{
            .size = size,
        };

        Graphics::CopyBuffer(stagingBuffer, _buffer, region);

        vmaDestroyBuffer(Graphics::Allocator, stagingBuffer, stagingAllocation);
    }
    else {
        auto bufferCreateInfo = VkBufferCreateInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = _size,
            .usage = _bufferUsage,
        };

        auto allocationCreateInfo = VmaAllocationCreateInfo{
            .usage = _memoryUsage,
        };

        if (data) {
            allocationCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
        }

        VmaAllocationInfo allocationInfo;
        result = vmaCreateBuffer(
            Graphics::Allocator,
            &bufferCreateInfo,
            &allocationCreateInfo,
            &_buffer,
            &_allocation,
            &allocationInfo
        );
        CheckVkResult("vmaCreateBuffer", result);
        
        _mappedMemory = reinterpret_cast<uint8_t *>(allocationInfo.pMappedData);

        if (data) {
            memcpy(_mappedMemory, data, _size);
            UnMap();
        }
    }
}

DUSK_API
void VulkanBuffer::Destroy()
{
    UnMap();

    if (_buffer) {
        vmaDestroyBuffer(Graphics::Allocator, _buffer, _allocation);
        _buffer = VK_NULL_HANDLE;
        _allocation = VK_NULL_HANDLE;
    }

    _size = 0;
    _bufferUsage = VkBufferUsageFlagBits(0);
    _memoryUsage = VMA_MEMORY_USAGE_UNKNOWN;
}

DUSK_API
void VulkanBuffer::Map()
{
    VkResult result;

    if (IsMapped()) {
        return;
    }

    void * pointer = nullptr;
    result = vmaMapMemory(Graphics::Allocator, _allocation, &pointer);
    CheckVkResult("vmaMapMemory", result);

    _mappedMemory = reinterpret_cast<uint8_t *>(pointer);
}

DUSK_API
void VulkanBuffer::UnMap()
{
    if (not IsMapped()) {
        return;
    }

    vmaUnmapMemory(Graphics::Allocator, _allocation);
    _mappedMemory = nullptr;
}

DUSK_API
void VulkanBuffer::ReadFrom(size_t offset, size_t length, void * data)
{
    assert(_memoryUsage == VMA_MEMORY_USAGE_GPU_TO_CPU);
    // assert(_mappedMemory);

    bool alreadyMapped = IsMapped();

    Map();

    memcpy(data, _mappedMemory + offset, length);

    if (not alreadyMapped) {
        UnMap();
    }
}

DUSK_API
void VulkanBuffer::WriteTo(size_t offset, size_t length, void * data)
{
    assert(_memoryUsage == VMA_MEMORY_USAGE_CPU_ONLY or _memoryUsage == VMA_MEMORY_USAGE_CPU_TO_GPU);
    // assert(_mappedMemory);

    bool alreadyMapped = IsMapped();
    
    Map();

    memcpy(_mappedMemory + offset, data, length);

    if (not alreadyMapped) {
        UnMap();
    }
}

} // namespace dusk