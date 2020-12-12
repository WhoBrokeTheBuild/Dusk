#ifndef DUSK_VULKAN_MESH_HPP
#define DUSK_VULKAN_MESH_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>

#include <Dusk/Mesh.hpp>

namespace Dusk::Vulkan {

#define DUSK_VULKAN_MESH(x) (dynamic_cast<Dusk::Vulkan::VulkanMesh *>(x))

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

    inline std::vector<VkVertexInputBindingDescription> GetVkVertexInputBindings() {
        return _bindings;
    }

    inline std::vector<VkVertexInputAttributeDescription> GetVkVertexInputAttributes() {
        return _attributes;
    }

    inline std::vector<VkBuffer> GetVkBuffers() {
        return _buffers;
    }

    inline uint32_t GetVertexCount() {
        return _vertexCount;
    }

private:

    bool AddBuffer(const void * data, VkDeviceSize size, uint32_t stride, 
        VkFormat format, VkVertexInputRate inputRate, VkBufferUsageFlags usage);

    std::vector<VkBuffer> _buffers;

    std::vector<VkDeviceMemory> _bufferMemories;

    std::vector<VkVertexInputBindingDescription> _bindings;

    std::vector<VkVertexInputAttributeDescription> _attributes;

    unsigned _nextLocation = 0;

    unsigned _nextBinding = 0;

    uint32_t _vertexCount = 0;

}; // class VulkanMesh

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_MESH_HPP