#include <Dusk/VulkanBuffer.hpp>
#include <Dusk/Exception.hpp>
#include <Dusk/Graphics.hpp>

namespace dusk {

DUSK_API
VulkanBuffer::~VulkanBuffer()
{
    Destroy();
}

DUSK_API
void VulkanBuffer::Create(vk::BufferUsageFlagBits bufferUsage, VmaMemoryUsage memoryUsage, vk::DeviceSize size, void * data /* = nullptr */)
{
    Destroy();

    _size = size;
    _bufferUsage = bufferUsage;
    _memoryUsage = memoryUsage;

    if (_memoryUsage == VMA_MEMORY_USAGE_GPU_ONLY) {
        if (not data) {
            throw Exception("Attempting to create a GPU only buffer with no data");
        }

        auto stagingBufferCreateInfo = vk::BufferCreateInfo()
            .setSize(_size)
            .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
            .setSharingMode(vk::SharingMode::eExclusive);

        auto stagingAllocationCreateInfo = VmaAllocationCreateInfo{
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_CPU_ONLY,
        };

        VmaAllocationInfo stagingAllocationInfo;
        auto[stagingBuffer, stagingAllocation] = Graphics::CreateBuffer(
            stagingBufferCreateInfo,
            stagingAllocationCreateInfo,
            &stagingAllocationInfo
        );

        memcpy(stagingAllocationInfo.pMappedData, data, _size);

        auto bufferCreateInfo = vk::BufferCreateInfo()
            .setSize(_size)
            .setUsage(vk::BufferUsageFlagBits::eTransferDst | _bufferUsage)
            .setSharingMode(vk::SharingMode::eExclusive);

        auto allocationCreateInfo = VmaAllocationCreateInfo{
            .usage = _memoryUsage,
        };

        std::tie(_buffer, _allocation) = Graphics::CreateBuffer(
            bufferCreateInfo,
            allocationCreateInfo
        );

        auto region = vk::BufferCopy()
            .setSize(size);

        Graphics::CopyBuffer(stagingBuffer, _buffer, region);

        Graphics::Device.destroyBuffer(stagingBuffer);
        vmaFreeMemory(Graphics::Allocator, stagingAllocation);
    }
    else {
        auto bufferCreateInfo = vk::BufferCreateInfo()
            .setSize(_size)
            .setUsage(_bufferUsage);

        auto allocationCreateInfo = VmaAllocationCreateInfo{
            .usage = _memoryUsage,
        };

        if (data) {
            allocationCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
        }

        VmaAllocationInfo allocationInfo;
        std::tie(_buffer, _allocation) = Graphics::CreateBuffer(
            bufferCreateInfo,
            allocationCreateInfo,
            &allocationInfo
        );

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

    vmaDestroyBuffer(Graphics::Allocator, _buffer, _allocation);
    _buffer = VK_NULL_HANDLE;
    _allocation = VK_NULL_HANDLE;

    _size = 0;
    _bufferUsage = vk::BufferUsageFlagBits(0);
    _memoryUsage = VMA_MEMORY_USAGE_UNKNOWN;
}

DUSK_API
void VulkanBuffer::Map()
{
    if (IsMapped()) {
        return;
    }

    void * pointer = nullptr;
    auto result = vmaMapMemory(Graphics::Allocator, _allocation, &pointer);
    
    vk::detail::resultCheck(vk::Result(result), "vmaMapMemory");

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