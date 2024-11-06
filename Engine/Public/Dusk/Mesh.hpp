#ifndef DUSK_MESH_HPP
#define DUSK_MESH_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/NonCopyable.hpp>
#include <Dusk/Span.hpp>
#include <Dusk/VulkanPrimitive.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>

#include <memory>

namespace dusk {

class DUSK_API Mesh : public NonCopyable
{
public:

    using Pointer = std::unique_ptr<Mesh>;

    Mesh() = default;

    inline Mesh(List<VulkanPrimitive::Pointer> && primitiveList)
        : _primitiveList(std::move(primitiveList))
    { }

    virtual ~Mesh() = default;

    inline BoundingBox GetBounds() const {
        BoundingBox bounds;
        for (const auto& primitive : _primitiveList) {
            bounds.Extend(primitive->GetBounds());
        }
        return bounds;
    }

    inline void AddPrimitive(VulkanPrimitive::Pointer && primitive) {
        _primitiveList.push_back(std::move(primitive));
    }

    // TODO: PrimitiveVertex::Color

    void AddPlane(
        vec3 center = { 0.0f, 0.0f, 0.0f },
        vec3 normal = { 0.0f, 1.0f, 0.0f }, // TODO: = UP
        vec2 size = { 1.0f, 1.0f },
        uvec2 subdivisions = { 1, 1 }
    );

    // void AddCube(vec3 topLeftFront, vec3 bottomRightBack);

    // void AddBox(vec3 topLeftFront, vec3 bottomRightBack);

    // void AddUVSphere(vec3 center, float radius, unsigned rows, unsigned columns);

    // void AddIcoSphere(vec3 center, float radius, unsigned subdivisons);
    
    inline void UpdateDescriptorSet(VulkanBuffer * transformBuffer) {
        for (const auto& primitive : _primitiveList) {
            primitive->UpdateDescriptorSet(transformBuffer);
        }
    }
    
    inline void GenerateCommands(VkCommandBuffer commandBuffer) {
        for (const auto& primitive : _primitiveList) {
            primitive->GenerateCommands(commandBuffer);
        }
    }

private:

    List<VulkanPrimitive::Pointer> _primitiveList;

}; // class Mesh

} // namespace dusk

#endif // DUSK_MESH_HPP