#ifndef DUSK_VULKAN_PRIMITIVE_HPP
#define DUSK_VULKAN_PRIMITIVE_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Primitive.hpp>
#include <Dusk/PrimitiveData.hpp>

#include <vector>
#include <optional>

namespace Dusk::Vulkan {

class DUSK_VULKAN_API VulkanPrimitive : public Primitive
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanPrimitive)

    VulkanPrimitive() = default;

    virtual ~VulkanPrimitive();

    bool Load(const std::unique_ptr<PrimitiveData>& data) override;

    VkPrimitiveTopology GetVkPrimitiveTopology() {
        return _vkPrimitiveTopology;
    }
    
    void GenerateBindCommands(VkCommandBuffer vkCommandBuffer);

    void GenerateDrawCommands(VkCommandBuffer vkCommandBuffer);

private:

    // bool AddBuffer(const void * data, VkDeviceSize size, uint32_t stride, VkFormat format,
    //     VkVertexInputRate inputRate, VkBufferUsageFlags usage, VertexAttribute attribute)

    VkBuffer _vkIndexBuffer;

    VkBuffer _vkVertexBuffer;

    std::vector<VkDeviceMemory> _vkBufferMemories;

    VkPrimitiveTopology _vkPrimitiveTopology;

    bool _indexed = false;

    uint32_t _count = 0;

}; // class VulkanPrimitive

inline std::optional<VkPrimitiveTopology> GetVkPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
    switch (primitiveTopology) {
    case PrimitiveTopology::Points:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case PrimitiveTopology::Lines:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case PrimitiveTopology::LineStrip:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case PrimitiveTopology::Triangles:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case PrimitiveTopology::TriangleStrip:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    }

    return {};
}

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_PRIMITIVE_HPP