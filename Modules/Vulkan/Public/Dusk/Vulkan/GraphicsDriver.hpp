#ifndef DUSK_VULKAN_GRAPHICS_DRIVER_HPP
#define DUSK_VULKAN_GRAPHICS_DRIVER_HPP

#include <Dusk/Vulkan/Config.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>

#include <SDL.h>

#include <vector>

namespace Dusk::Vulkan {

class DUSK_VULKAN_API GraphicsDriver : public IGraphicsDriver
{
public:

    GraphicsDriver();

    virtual ~GraphicsDriver();

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::unique_ptr<ITexture> CreateTexture() override;

    std::unique_ptr<IShader> CreateShader() override;
    
    std::unique_ptr<IMesh> CreateMesh() override;

private:

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

    std::vector<VkCommandBuffer> _vkPresentCommandBuffers;

}; // class GraphicsDriver

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_GRAPHICS_DRIVER_HPP
