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

    void Create(vk::BufferUsageFlagBits bufferUsage, VmaMemoryUsage memoryUsage, Span<uint8_t> data);

    void Create(vk::BufferUsageFlagBits bufferUsage, VmaMemoryUsage memoryUsage, vk::DeviceSize size, void * data = nullptr);

    void Destroy();

    inline vk::DeviceSize GetSize() const {
        return _size;
    }

    inline vk::BufferUsageFlagBits GetBufferUsage() const {
        return _bufferUsage;
    }

    inline VmaMemoryUsage GetMemoryUsage() const {
        return _memoryUsage;
    }

    inline vk::Buffer GetVkBuffer() const {
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

    vk::DeviceSize _size;

    vk::BufferUsageFlagBits _bufferUsage;

    VmaMemoryUsage _memoryUsage;

    vk::Buffer _buffer = VK_NULL_HANDLE;

    VmaAllocation _allocation = VK_NULL_HANDLE;

    uint8_t * _mappedMemory = nullptr;

}; // VulkanBuffer

} // namespace dusk

#endif // DUSK_GRAPHICS_BUFFER_HPP