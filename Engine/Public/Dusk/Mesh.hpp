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

    inline void AddPrimitive(VulkanPrimitive::Pointer && primitive) {
        _primitiveList.push_back(std::move(primitive));
    }
    
    inline void GenerateCommands(vk::CommandBuffer commandBuffer) {
        for (const auto& primitive : _primitiveList) {
            primitive->GenerateCommands(commandBuffer);
        }
    }

private:

    List<VulkanPrimitive::Pointer> _primitiveList;

}; // class Mesh

} // namespace dusk

#endif // DUSK_MESH_HPP