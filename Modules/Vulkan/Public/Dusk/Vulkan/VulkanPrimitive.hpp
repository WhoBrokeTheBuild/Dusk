#ifndef DUSK_VULKAN_PRIMITIVE_HPP
#define DUSK_VULKAN_PRIMITIVE_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>

#include <Dusk/Primitive.hpp>
#include <Dusk/PrimitiveData.hpp>

namespace Dusk::Vulkan {

class DUSK_VULKAN_API VulkanPrimitive : public Primitive
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanPrimitive)

    VulkanPrimitive() = default;

    virtual ~VulkanPrimitive();

    bool Load(const std::unique_ptr<PrimitiveData>& data) override;

    inline std::vector<VkVertexInputBindingDescription> GetVkVertexInputBindings() {
        return _vkBindings;
    }

    inline std::vector<VkVertexInputAttributeDescription> GetVkVertexInputAttributes() {
        return _vkAttributes;
    }

    VkPrimitiveTopology GetVkPrimitiveTopology() {
        return _vkPrimitiveTopology;
    }

private:

    // bool AddBuffer(const void * data, VkDeviceSize size, uint32_t stride, VkFormat format,
    //     VkVertexInputRate inputRate, VkBufferUsageFlags usage, VertexAttribute attribute)

    std::vector<VkBuffer> _vkBuffers;

    std::vector<VkDeviceMemory> _vkBufferMemories;

    std::vector<VkVertexInputBindingDescription> _vkBindings;

    std::vector<VkVertexInputAttributeDescription> _vkAttributes;

    VkPrimitiveTopology _vkPrimitiveTopology;

    bool _indexed = false;

    uint32_t _count = 0;

}; // class VulkanPrimitive

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_PRIMITIVE_HPP