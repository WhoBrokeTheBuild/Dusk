#ifndef DUSK_VULKAN_RENDER_CONTEXT_HPP
#define DUSK_VULKAN_RENDER_CONTEXT_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>
#include <Dusk/RenderContext.hpp>

namespace Dusk::Vulkan {

#define DUSK_VULKAN_RENDER_CONTEXT(x) (dynamic_cast<Dusk::Vulkan::VulkanRenderContext *>(x))

class DUSK_ENGINE_API VulkanRenderContext : public RenderContext
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanRenderContext)

    VulkanRenderContext() = default;

    virtual ~VulkanRenderContext() = default;

    inline void SetVkCommandBuffer(VkCommandBuffer vkCommandBuffer) {
        _vkCommandBuffer = vkCommandBuffer;
    }

    inline VkCommandBuffer GetVkCommandBuffer() {
        return _vkCommandBuffer;
    }

private:

    VkCommandBuffer _vkCommandBuffer;

}; // class RenderContext

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_RENDER_CONTEXT_HPP