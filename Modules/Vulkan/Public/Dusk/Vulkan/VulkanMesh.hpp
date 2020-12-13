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
        return _vkBindings;
    }

    inline std::vector<VkVertexInputAttributeDescription> GetVkVertexInputAttributes() {
        return _vkAttributes;
    }

    VkPrimitiveTopology GetVkPrimitiveTopology() {
        return _vkPrimitiveTopology;
    }

    void GenerateBindCommands(VkCommandBuffer vkCommandBuffer);
    
    void GenerateDrawCommands(VkCommandBuffer vkCommandBuffer);

private:

    bool AddBuffer(const void * data, VkDeviceSize size, uint32_t stride, 
        VkFormat format, VkVertexInputRate inputRate, VkBufferUsageFlags usage);

    std::vector<VkBuffer> _vkBuffers;

    std::vector<VkDeviceMemory> _vkBufferMemories;

    std::vector<VkVertexInputBindingDescription> _vkBindings;

    std::vector<VkVertexInputAttributeDescription> _vkAttributes;

    VkPrimitiveTopology _vkPrimitiveTopology;

    unsigned _nextLocation = 0;

    unsigned _nextBinding = 0;

    bool _indexed = 0;

    uint32_t _count = 0;

}; // class VulkanMesh

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_MESH_HPP