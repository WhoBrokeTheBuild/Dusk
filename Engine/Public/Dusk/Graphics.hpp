#ifndef DUSK_GRAPHICS_HPP
#define DUSK_GRAPHICS_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Exception.hpp>
#include <Dusk/ShaderGlobals.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Tuple.hpp>
#include <Dusk/VulkanBuffer.hpp>
#include <Dusk/VulkanUtil.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>
#include <Dusk/ThirdParty/SDL.hpp>


#include <functional>

namespace dusk {
    
namespace Graphics {

    DUSK_API
    extern VkInstance Instance;

    DUSK_API
    extern VkDevice Device;

    DUSK_API
    extern VkPhysicalDeviceProperties PhysicalDeviceProperties;

    DUSK_API
    extern VkPhysicalDeviceFeatures PhysicalDeviceFeatures;

    DUSK_API
    extern VmaAllocator Allocator;

    DUSK_API
    extern VkRenderPass RenderPass;

    DUSK_API
    extern ShaderGlobals Globals;

    DUSK_API
    extern VulkanBuffer::Pointer GlobalsBuffer;

    DUSK_API
    extern VkDescriptorPool DescriptorPool;

    DUSK_API
    extern VkDescriptorSetLayout DescriptorSetLayout;

    DUSK_API
    extern VkPipelineLayout PipelineLayout;

    DUSK_API
    void Init();

    DUSK_API
    void Term();

    DUSK_API
    void Render();

    DUSK_API
    void SetRenderCallback(std::function<void(VkCommandBuffer)> renderCallback);

    DUSK_API
    void HandleEvent(SDL_Event * event);

    // void SetWindowTitle(String windowTitle);

    // void SetWindowSize(unsigned width, unsigned height);

    DUSK_API
    VkExtent2D GetWindowSize();

    DUSK_API
    void CopyBuffer(VkBuffer source, VkBuffer destination, VkBufferCopy region);

    DUSK_API
    void CopyBufferToImage(VkBuffer source, VkImage destination, VkBufferImageCopy region);

} // namespace Graphics

} // namespace dusk

#endif // DUSK_GRAPHICS_HPP