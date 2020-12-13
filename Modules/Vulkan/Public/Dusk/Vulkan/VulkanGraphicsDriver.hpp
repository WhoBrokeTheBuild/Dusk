#ifndef DUSK_VULKAN_GRAPHICS_DRIVER_HPP
#define DUSK_VULKAN_GRAPHICS_DRIVER_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>

#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/Vulkan/VulkanPipeline.hpp>
#include <Dusk/Vulkan/VulkanTexture.hpp>
#include <Dusk/Vulkan/VulkanShader.hpp>
#include <Dusk/Vulkan/VulkanMesh.hpp>

#include <vector>

namespace Dusk::Vulkan {

#define DUSK_VULKAN_GRAPHICS_DRIVER(x) (dynamic_cast<Dusk::Vulkan::VulkanGraphicsDriver *>(x))

class DUSK_VULKAN_API VulkanGraphicsDriver : public GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanGraphicsDriver)

    VulkanGraphicsDriver() = default;

    virtual ~VulkanGraphicsDriver() = default;

    inline std::string GetClassID() const override {
        return "DuskGraphicsDriver";
    }

    bool Initialize() override;

    void Terminate() override;

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh) override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;
    
    std::shared_ptr<Mesh> CreateMesh() override;

    bool SetShaderData(const std::string& name, size_t size, void * data) override;


    inline VkDevice GetVkDevice() const {
        return _vkDevice;
    }

    inline VkExtent2D GetVkSwapChainExtent() const {
        return _vkSwapChainExtent;
    }

    inline VkPipelineLayout GetVkPipelineLayout() {
        return _vkPipelineLayout;
    }

    inline VkRenderPass GetVkRenderPass() const {
        return _vkRenderPass;
    }

    uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags props);

    bool CreateBuffer(VkBuffer & buffer, VkDeviceMemory & memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props);


private:

    bool IsDeviceSuitable(const VkPhysicalDevice device);

    std::vector<const char *> GetEnabledDeviceLayers();

    std::vector<const char *> GetEnabledDeviceExtensions();

    std::vector<const char *> GetRequiredInstanceExtensions();

    bool InitWindow();

    void TermWindow();

    bool InitDebugMessenger();

    void TermDebugMessenger();

    bool InitInstance();

    void TermInstance();
    
    bool InitSurface();

    void TermSurface();
    
    bool InitPhysicalDevice();

    bool InitLogicalDevice();

    void TermLogicalDevice();
    
    bool InitSwapChain();

    void TermSwapChain();

    bool ResetSwapChain();

    bool InitRenderPass();

    bool InitDescriptorPool();

    bool InitGraphicsPipelines();

    bool InitFramebuffers();

    bool InitCommandPool();

    bool InitDepthBuffer();

    bool InitCommandBuffers();

    bool InitSyncObjects();

    SDL_Window * _sdlWindow = nullptr;

    VkInstance _vkInstance;

    VkDebugUtilsMessengerEXT _vkDebugMessenger;
    bool _debugMessengerInitialized = false;

    VkSurfaceKHR _vkSurface;

    VkPhysicalDeviceProperties _vkPhysicalDeviceProperties;
    
    VkPhysicalDeviceFeatures _vkPhysicalDeviceFeatures;

    VkPhysicalDevice _vkPhysicalDevice;

    VkDevice _vkDevice;

    uint32_t _vkGraphicsQueueFamilyIndex;
    uint32_t _vkPresentQueueFamilyIndex;

    VkQueue _vkGraphicsQueue;
    VkQueue _vkPresentQueue;

    VkSurfaceFormatKHR _vkSwapChainImageFormat;

    VkExtent2D _vkSwapChainExtent;

    VkSwapchainKHR _vkSwapChain;

    std::vector<VkImage> _vkSwapChainImages;

    std::vector<VkImageView> _vkSwapChainImageViews;

    VkRenderPass _vkRenderPass;

    VkFormat _vkDepthImageFormat;

    VkImage _vkDepthImage;

    VkDeviceMemory _vkDepthImageMemory;

    VkImageView _vkDepthImageView;

    std::vector<VkFramebuffer> _vkFramebuffers;

    VkCommandPool _vkCommandPool;

    std::vector<VkCommandBuffer> _vkCommandBuffers;

    std::vector<VkSemaphore> _vkImageAvailableSemaphores;
    
    std::vector<VkSemaphore> _vkRenderingFinishedSemaphores;

    std::vector<VkFence> _vkInFlightFences;

    std::vector<VkFence> _vkImagesInFlight;

    std::vector<VkBuffer> _vkUniformBuffers;
    std::vector<VkDeviceMemory> _vkUniformBuffersMemory;

    VkDescriptorPool _vkDescriptorPool;

    VkDescriptorSetLayout _vkDescriptorSetLayout;

    VkPipelineLayout _vkPipelineLayout;

    std::vector<VkDescriptorSet> _vkDescriptorSets;

    int _currentFrame = 0;

    std::vector<std::shared_ptr<Pipeline>> _pipelines;



    // bool InitGraphicsPipeline();

    // void TermGraphicsPipeline();

    

}; // class VulkanGraphicsDriver

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_GRAPHICS_DRIVER_HPP
