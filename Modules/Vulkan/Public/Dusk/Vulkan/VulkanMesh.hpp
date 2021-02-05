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

    virtual ~VulkanMesh() = default;

    bool Initialize() override;

    void Terminate() override;

    bool Create();

    void Destroy();

    void Render(RenderContext * ctx) override;
    
private:

    VkDescriptorSet _vkDescriptorSet;

}; // class VulkanMesh

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_MESH_HPP