#ifndef DUSK_VULKAN_GRAPHICS_DRIVER_HPP
#define DUSK_VULKAN_GRAPHICS_DRIVER_HPP

#include <Dusk/Vulkan/Config.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Vulkan/Pipeline.hpp>
#include <Dusk/Vulkan/Texture.hpp>
#include <Dusk/Vulkan/Shader.hpp>
#include <Dusk/Vulkan/Mesh.hpp>

#include <SDL.h>

#include <vector>

namespace Dusk {

#define DUSK_VULKAN_GRAPHICS_DRIVER(x) (dynamic_cast<Dusk::VulkanGraphicsDriver *>(x))

class DUSK_VULKAN_API VulkanGraphicsDriver : public GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanGraphicsDriver)

    VulkanGraphicsDriver();

    virtual ~VulkanGraphicsDriver();

    inline std::string GetClassID() const override {
        return "DuskGraphicsDriver";
    }

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::shared_ptr<Dusk::Pipeline> CreatePipeline() override;

    std::shared_ptr<Dusk::Texture> CreateTexture() override;

    std::shared_ptr<Dusk::Shader> CreateShader() override;
    
    std::shared_ptr<Dusk::Mesh> CreateMesh() override;

    bool SetShaderData(const std::string& name, size_t size, void * data) override;


    VkDevice GetDevice() const {
        return _vkDevice;
    }

    uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags props);

    bool CreateBuffer(VkBuffer & buffer, VkDeviceMemory & memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props);

private:

    bool IsDeviceSuitable(const VkPhysicalDevice device);

    SDL_Window * _sdlWindow = nullptr;

    VkInstance _vkInstance;

    VkSurfaceKHR _vkWindowSurface;

    VkPhysicalDevice _vkPhysicalDevice;

    VkDevice _vkDevice;

    VkQueue _vkGraphicsQueue;
    VkQueue _vkPresentQueue;

    VkSemaphore _vkImageAvailableSemaphore;
    VkSemaphore _vkRenderingFinishedSemaphore;

    VkSwapchainKHR _vkSwapChain;

    std::vector<VkImage> _vkSwapChainImages;

    VkCommandPool _vkCommandPool;

    std::vector<VkCommandBuffer> _vkCommandBuffers;

}; // class VulkanGraphicsDriver

} // namespace Dusk

#endif // DUSK_VULKAN_GRAPHICS_DRIVER_HPP
