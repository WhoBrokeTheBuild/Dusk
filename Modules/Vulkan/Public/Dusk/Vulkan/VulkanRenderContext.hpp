#ifndef DUSK_VULKAN_RENDER_CONTEXT_HPP
#define DUSK_VULKAN_RENDER_CONTEXT_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>
#include <Dusk/RenderContext.hpp>

namespace Dusk {

class DUSK_ENGINE_API VulkanRenderContext : public Object
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

} // namespace Dusk

#endif // DUSK_VULKAN_RENDER_CONTEXT_HPP