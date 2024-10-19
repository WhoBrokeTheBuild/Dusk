#ifndef DUSK_GRAPHICS_BUFFER_HPP
#define DUSK_GRAPHICS_BUFFER_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Span.hpp>
#include <Dusk/List.hpp>
#include <Dusk/NonCopyable.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>

#include <memory>

namespace dusk {

class DUSK_API VulkanBuffer : NonCopyable
{
public:

    using Pointer = std::shared_ptr<VulkanBuffer>;

    VulkanBuffer() = default;

    virtual ~VulkanBuffer();

    void Create(VkBufferUsageFlagBits bufferUsage, VmaMemoryUsage memoryUsage, Span<uint8_t> data);

    void Create(VkBufferUsageFlagBits bufferUsage, VmaMemoryUsage memoryUsage, VkDeviceSize size, void * data = nullptr);

    void Destroy();

    inline VkDeviceSize GetSize() const {
        return _size;
    }

    inline VkBufferUsageFlagBits GetBufferUsage() const {
        return _bufferUsage;
    }

    inline VmaMemoryUsage GetMemoryUsage() const {
        return _memoryUsage;
    }

    inline VkBuffer GetVkBuffer() const {
        return _buffer;
    }

    inline bool IsMapped() const {
        return (_mappedMemory != nullptr);
    }

    void Map();

    void UnMap();

    void ReadFrom(size_t offset, size_t length, void * data);

    void WriteTo(size_t offset, size_t length, void * data);

    inline void ReadFrom(size_t offset, Span<uint8_t> data) {
        return ReadFrom(offset, data.size(), data.data());
    }

    inline void WriteTo(size_t offset, Span<uint8_t> data) {
        return WriteTo(offset, data.size(), data.data());
    }

private:

    VkDeviceSize _size;

    VkBufferUsageFlagBits _bufferUsage;

    VmaMemoryUsage _memoryUsage;

    VkBuffer _buffer = VK_NULL_HANDLE;

    VmaAllocation _allocation = VK_NULL_HANDLE;

    uint8_t * _mappedMemory = nullptr;

}; // VulkanBuffer

} // namespace dusk

#endif // DUSK_GRAPHICS_BUFFER_HPP