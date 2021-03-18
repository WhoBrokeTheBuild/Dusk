#ifndef DUSK_VULKAN_PRIMITIVE_HPP
#define DUSK_VULKAN_PRIMITIVE_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>

#include <Dusk/Primitive.hpp>
#include <Dusk/Vulkan/VulkanBuffer.hpp>

#include <optional>

namespace Dusk::Vulkan {

#define DUSK_VULKAN_PRIMITIVE(x) (dynamic_cast<Dusk::Vulkan::VulkanPrimitive *>(x))

class DUSK_VULKAN_API VulkanPrimitive : public Primitive
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanPrimitive)

    VulkanPrimitive() = default;

    virtual ~VulkanPrimitive() = default;

    void Terminate() override;

    bool Load(const std::unique_ptr<PrimitiveData>& data) override;

    VkPrimitiveTopology GetVkPrimitiveTopology() {
        return _vkPrimitiveTopology;
    }
    
    void GenerateCommands(VkCommandBuffer vkCommandBuffer);

private:

    std::unique_ptr<VulkanBuffer> _indexBuffer;

    std::unique_ptr<VulkanBuffer> _vertexBuffer;

    VkPrimitiveTopology _vkPrimitiveTopology;

    bool _indexed = false;

    uint32_t _count = 0;

}; // class VulkanPrimitive

inline std::optional<VkPrimitiveTopology> GetVkPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
    switch (primitiveTopology) {
    case PrimitiveTopology::PointList:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case PrimitiveTopology::LineList:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case PrimitiveTopology::LineStrip:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case PrimitiveTopology::TriangleList:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case PrimitiveTopology::TriangleStrip:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    }

    return {};
}

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_PRIMITIVE_HPP