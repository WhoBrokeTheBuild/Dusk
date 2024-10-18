#ifndef DUSK_GRAPHICS_HPP
#define DUSK_GRAPHICS_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Tuple.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>
#include <Dusk/ThirdParty/SDL.hpp>

#include <Dusk/ShaderGlobals.hpp>
#include <Dusk/VulkanBuffer.hpp>

#include <functional>

namespace dusk {
    
namespace Graphics {

    DUSK_API
    extern vk::Instance Instance;

    DUSK_API
    extern vk::Device Device;

    DUSK_API
    extern vk::PhysicalDeviceProperties PhysicalDeviceProperties;

    DUSK_API
    extern vk::PhysicalDeviceFeatures PhysicalDeviceFeatures;

    DUSK_API
    extern VmaAllocator Allocator;

    DUSK_API
    extern vk::RenderPass RenderPass;

    DUSK_API
    extern ShaderGlobals Globals;

    DUSK_API
    extern VulkanBuffer::Pointer GlobalsBuffer;

    DUSK_API
    extern vk::DescriptorPool DescriptorPool;

    DUSK_API
    extern vk::DescriptorSetLayout DescriptorSetLayout;

    DUSK_API
    extern vk::PipelineLayout PipelineLayout;

    DUSK_API
    void Init();

    DUSK_API
    void Term();

    DUSK_API
    void Render();

    DUSK_API
    void SetRenderCallback(std::function<void(vk::CommandBuffer)> renderCallback);

    DUSK_API
    void HandleEvent(SDL_Event * event);

    // void SetWindowTitle(String windowTitle);

    // void SetWindowSize(unsigned width, unsigned height);

    DUSK_API
    vk::Extent2D GetWindowSize();

    DUSK_API
    Tuple<vk::Buffer, VmaAllocation> CreateBuffer(
        vk::BufferCreateInfo& bufferCreateInfo,
        VmaAllocationCreateInfo& allocationCreateInfo,
        VmaAllocationInfo * allocationInfo = nullptr
    );

    DUSK_API
    Tuple<vk::Image, VmaAllocation> CreateImage(
        vk::ImageCreateInfo& imageCreateInfo,
        VmaAllocationCreateInfo& allocationCreateInfo,
        VmaAllocationInfo * allocationInfo = nullptr
    );

    DUSK_API
    void CopyBuffer(vk::Buffer source, vk::Buffer destination, vk::BufferCopy region);

    DUSK_API
    void CopyBufferToImage(vk::Buffer source, vk::Image destination, vk::BufferImageCopy region);

} // namespace Graphics

} // namespace dusk

#endif // DUSK_GRAPHICS_HPP