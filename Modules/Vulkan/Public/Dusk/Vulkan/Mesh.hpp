#ifndef DUSK_VULKAN_MESH_HPP
#define DUSK_VULKAN_MESH_HPP

#include <Dusk/Vulkan/Config.hpp>
#include <Dusk/Graphics/Mesh.hpp>

namespace Dusk {

class DUSK_VULKAN_API VulkanMesh : public Mesh
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanMesh)

    VulkanMesh() = default;

    virtual ~VulkanMesh();

    inline std::string GetClassID() const override {
        return "Dusk::VulkanMesh";
    }

    void Render() override;

    bool Load(const MeshData * data) override;

private:

    bool AddBuffer(void * data, VkDeviceSize size, uint32_t stride, 
        VkFormat format, VkVertexInputRate inputRate, VkBufferUsageFlags usage);

    std::vector<VkBuffer> _buffers;

    std::vector<VkDeviceMemory> _bufferMemories;

    std::vector<VkVertexInputBindingDescription> _bindings;

    std::vector<VkVertexInputAttributeDescription> _attributes;

    unsigned _nextLocation = 0;

    unsigned _nextBinding = 0;

}; // class VulkanMesh

} // namespace Dusk

#endif // DUSK_VULKAN_MESH_HPP