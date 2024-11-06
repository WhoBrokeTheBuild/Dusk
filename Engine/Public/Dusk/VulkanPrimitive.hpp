#ifndef DUSK_VULKAN_PRIMITIVE_HPP
#define DUSK_VULKAN_PRIMITIVE_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/List.hpp>
#include <Dusk/NonCopyable.hpp>
#include <Dusk/PrimitiveVertex.hpp>
#include <Dusk/VulkanBuffer.hpp>
#include <Dusk/Material.hpp>
#include <Dusk/BoundingBox.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>

#include <memory>

namespace dusk {

class DUSK_API VulkanPrimitive : NonCopyable
{
public:

    using Pointer = std::unique_ptr<VulkanPrimitive>;

    VulkanPrimitive();

    virtual ~VulkanPrimitive();

    inline void SetMaterial(Material::Pointer material) {
        _material = material;
    }

    inline VkPrimitiveTopology GetTopology() const {
        return _topology;
    }

    inline BoundingBox GetBounds() const {
        return _bounds;
    }

    bool Create(
        Span<PrimitiveVertex> vertexList,
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        bool haveTangents = true
    );

    bool Create(
        Span<uint32_t> indexList,
        Span<PrimitiveVertex> vertexList,
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        bool haveTangents = true
    );

    void Destroy();

    void UpdateDescriptorSet(VulkanBuffer * transformBuffer);

    void GenerateCommands(VkCommandBuffer commandBuffer);

private:

    void calculateBounds(Span<PrimitiveVertex> vertexList);

    void calculateTangents(Span<uint32_t> indexList, Span<PrimitiveVertex> vertexList);

    bool _indexed = false;

    uint32_t _count;

    VkPrimitiveTopology _topology;
    
    Material::Pointer _material;

    VulkanBuffer _indexBuffer;

    VulkanBuffer _vertexBuffer;

    VkDescriptorSet _descriptorSet;

    BoundingBox _bounds;

}; // class VulkanPrimitive

} // namespace dusk

#endif // DUSK_VULKAN_PRIMITIVE_HPP