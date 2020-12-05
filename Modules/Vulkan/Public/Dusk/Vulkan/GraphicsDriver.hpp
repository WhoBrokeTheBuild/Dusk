#ifndef DUSK_VULKAN_GRAPHICS_DRIVER_HPP
#define DUSK_VULKAN_GRAPHICS_DRIVER_HPP

#include <Dusk/Vulkan/Config.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>

#include <SDL.h>

#include <vector>

namespace Dusk::Vulkan {

class DUSK_VULKAN_API GraphicsDriver : public Dusk::GraphicsDriver
{
public:

    GraphicsDriver();

    virtual ~GraphicsDriver();

    inline std::string GetClassID() const override {
        return "Dusk::Vulkan::GraphicsDriver";
    }

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::shared_ptr<Dusk::Texture> CreateTexture() override;

    std::shared_ptr<Dusk::Shader> CreateShader() override;
    
    std::shared_ptr<Dusk::Mesh> CreateMesh() override;

    bool SetShaderData(const std::string& name, size_t size, void * data) override;

    VkDevice GetDevice() const {
        return _vkDevice;
    }

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

    std::vector<VkCommandBuffer> _vkCommandBuffers;

}; // class GraphicsDriver

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_GRAPHICS_DRIVER_HPP
