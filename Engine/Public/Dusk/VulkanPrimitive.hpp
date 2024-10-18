#ifndef DUSK_VULKAN_PRIMITIVE_HPP
#define DUSK_VULKAN_PRIMITIVE_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/List.hpp>
#include <Dusk/NonCopyable.hpp>
#include <Dusk/PrimitiveVertex.hpp>
#include <Dusk/VulkanBuffer.hpp>
#include <Dusk/Material.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>

#include <memory>

namespace dusk {

class DUSK_API VulkanPrimitive : NonCopyable
{
public:

    using Pointer = std::unique_ptr<VulkanPrimitive>;

    VulkanPrimitive() = default;

    virtual ~VulkanPrimitive();

    inline void SetMaterial(Material::Pointer material) {
        _material = material;
    }

    inline vk::PrimitiveTopology GetTopology() const {
        return _topology;
    }

    bool Create(
        Span<PrimitiveVertex> vertexList,
        vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList,
        bool haveTangents = true
    );

    bool Create(
        Span<uint32_t> indexList,
        Span<PrimitiveVertex> vertexList,
        vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList,
        bool haveTangents = true
    );

    void Destroy();

    void GenerateCommands(vk::CommandBuffer commandBuffer);

private:

    void calculateTangents(Span<uint32_t> indexList, Span<PrimitiveVertex> vertexList);

    bool _indexed = false;

    uint32_t _count;

    vk::PrimitiveTopology _topology;
    
    Material::Pointer _material;

    VulkanBuffer _indexBuffer;

    VulkanBuffer _vertexBuffer;

}; // class VulkanPrimitive

} // namespace dusk

#endif // DUSK_VULKAN_PRIMITIVE_HPP