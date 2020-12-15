#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/MeshImporter.hpp>
#include <Dusk/Camera.hpp>

#include <set>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
bool VulkanGraphicsDriver::Initialize()
{
    DuskBenchmarkStart();

    SDL2GraphicsDriver::Initialize();
    
    SDL2GraphicsDriver::CreateWindow(SDL_WINDOW_VULKAN);

    int vkVersion = gladLoaderLoadVulkan(nullptr, nullptr, nullptr);
    if (vkVersion == 0) {
        DuskLogError("gladLoaderLoadVulkan() failed");
        return false;
    }
    
    DuskLogVerbose("Vulkan %d.%d", GLAD_VERSION_MAJOR(vkVersion), GLAD_VERSION_MINOR(vkVersion));

    if (!InitInstance()) {
        return false;
    }

    InitDebugMessenger();

    if (!InitSurface()) {
        return false;
    }

    if (!InitPhysicalDevice()) {
        return false;
    }

    if (!InitLogicalDevice()) {
        return false;
    }

    if (!InitSwapChain()) {
        return false;
    }

    if (!InitRenderPass()) {
        return false;
    }

    if (!InitDescriptorPool()) {
        return false;
    }

    if (!InitGraphicsPipelines()) {
        return false;
    }

    if (!InitDepthBuffer()) {
        return false;
    }

    if (!InitFramebuffers()) {
        return false;
    }

    if (!InitCommandPool()) {
        return false;
    }

    if (!InitCommandBuffers()) {
        return false;
    }

    if (!InitSyncObjects()) {
        return false;
    }

    DuskBenchmarkEnd("VulkanGraphicsDriver::Initialize");
    return true;
}

DUSK_VULKAN_API
void VulkanGraphicsDriver::Terminate()
{
    DuskBenchmarkStart();

    vkDeviceWaitIdle(_vkDevice);

    TermSwapChain();
    _pipelines.clear();

    TermLogicalDevice();
    TermSurface();
    TermDebugMessenger();
    TermInstance();

    SDL2GraphicsDriver::Terminate();

    DuskBenchmarkEnd("VulkanGraphicsDriver::Terminate");
}

DUSK_VULKAN_API
void VulkanGraphicsDriver::SwapBuffers()
{
    VkResult vkResult;

    vkWaitForFences(_vkDevice, 1, &_vkInFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex = 0;
    vkResult = vkAcquireNextImageKHR(_vkDevice, _vkSwapChain, UINT64_MAX, _vkImageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (vkResult == VK_ERROR_OUT_OF_DATE_KHR) {
        if (!ResetSwapChain()) {
            DuskLogFatal("Failed to resize swap chain");
        }
    }
    else if (vkResult != VK_SUCCESS && vkResult != VK_SUBOPTIMAL_KHR) {
        DuskLogFatal("vkAcquireNextImageKHR() failed");
    }


    Camera camera;
    camera.SetPosition({ 3, 3, 3 });
    camera.SetLookAt({ 0, 0, 0 });

    static float rotation = 0.0f;
    rotation += 0.01f;

    TransformData transform = {
        .Model = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 1.0f, 0.0f)),
        .View = camera.GetView(),
        .Projection = camera.GetProjection(),
    };

    // OpenGL, and thus glm are upside-down
    transform.Projection[1][1] *= -1;

    transform.UpdateMVP();

    void * data;
    vkMapMemory(_vkDevice, _vkUniformBuffersMemory[imageIndex], 0, sizeof(transform), 0, &data);
    memcpy(data, &transform, sizeof(transform));
    vkUnmapMemory(_vkDevice, _vkUniformBuffersMemory[imageIndex]);



    if (_vkImagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(_vkDevice, 1, &_vkImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }

    _vkImagesInFlight[imageIndex] = _vkInFlightFences[_currentFrame];

    VkSemaphore waitSemaphores[] = { _vkImageAvailableSemaphores[_currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSemaphore signalSemaphores[] = { _vkRenderingFinishedSemaphores[_currentFrame] };

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &_vkCommandBuffers[imageIndex],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signalSemaphores,
    };

    vkResetFences(_vkDevice, 1, &_vkInFlightFences[_currentFrame]);

    vkResult = vkQueueSubmit(_vkGraphicsQueue, 1, &submitInfo, _vkInFlightFences[_currentFrame]);
    if (vkResult != VK_SUCCESS) {
        DuskLogFatal("vkQueueSubmit() failed");
    }

    VkSwapchainKHR swapChains[] = { _vkSwapChain };

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = 0,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signalSemaphores,
        .swapchainCount = 1,
        .pSwapchains = swapChains,
        .pImageIndices = &imageIndex,
    };

    vkResult = vkQueuePresentKHR(_vkPresentQueue, &presentInfo);

    if (vkResult == VK_ERROR_OUT_OF_DATE_KHR || vkResult == VK_SUBOPTIMAL_KHR) {
        if (!ResetSwapChain()) {
            DuskLogFatal("Failed to resize swap chain");
        }
    }
    else if (vkResult != VK_SUCCESS) {
        DuskLogFatal("vkQueuePresentKHR() failed");
    }

    _currentFrame = (_currentFrame + 1) % 2;
}

DUSK_VULKAN_API
std::shared_ptr<Pipeline> VulkanGraphicsDriver::CreatePipeline(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh)
{
    auto ptr = std::shared_ptr<Pipeline>(New VulkanPipeline());
    ptr->SetShader(shader);
    ptr->SetMesh(mesh);
    // ptr->Initialize();
    _pipelines.push_back(ptr);

    ResetSwapChain();
    return ptr;
}

DUSK_VULKAN_API
std::shared_ptr<Texture> VulkanGraphicsDriver::CreateTexture()
{
    return std::shared_ptr<Texture>(New VulkanTexture());
}

DUSK_VULKAN_API
std::shared_ptr<Shader> VulkanGraphicsDriver::CreateShader()
{
    return std::shared_ptr<Shader>(New VulkanShader());
}

DUSK_VULKAN_API
std::shared_ptr<Mesh> VulkanGraphicsDriver::CreateMesh()
{
    return std::shared_ptr<Mesh>(New VulkanMesh());
}

DUSK_VULKAN_API
bool VulkanGraphicsDriver::SetShaderData(const std::string& name, size_t size, void * buffer)
{



    return true;
}

uint32_t VulkanGraphicsDriver::FindMemoryType(uint32_t filter, VkMemoryPropertyFlags props)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(_vkPhysicalDevice, &memoryProperties);

    for (unsigned i = 0; i < memoryProperties.memoryTypeCount; ++i) {
        if ((filter & (1 << i)) && 
            (memoryProperties.memoryTypes[i].propertyFlags & props) == props) {
            return i;
        }
    }

    DuskLogError("Failed to find suitable memory type");
    return UINT32_MAX;
}

bool VulkanGraphicsDriver::CreateBuffer(VkBuffer & buffer, VkDeviceMemory & memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props)
{
    VkResult result;
    
    VkBufferCreateInfo bufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    result = vkCreateBuffer(_vkDevice, &bufferCreateInfo, nullptr, &buffer);
    if (result != VK_SUCCESS) {
        DuskLogError("Failed to create buffer");
        return false;
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(_vkDevice, buffer, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, props),
    };

    result = vkAllocateMemory(_vkDevice, &memoryAllocateInfo, nullptr, &memory);
    if (result != VK_SUCCESS) {
        DuskLogError("Failed to allocate memory");
        return false;
    }

    vkBindBufferMemory(_vkDevice, buffer, memory, 0);
    return true;
}

bool VulkanGraphicsDriver::IsDeviceSuitable(const VkPhysicalDevice device)
{
    vkGetPhysicalDeviceProperties(device, &_vkPhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(device, &_vkPhysicalDeviceFeatures);

    // TODO:
    // * Queue Families
    // * Device Extensions
    // * Swap Chain Support

    return _vkPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
        && _vkPhysicalDeviceFeatures.geometryShader;
}

std::vector<const char *> VulkanGraphicsDriver::GetEnabledDeviceLayers()
{
    std::vector<const char *> enabledLayers = {

    };

    #if defined(DUSK_VULKAN_VALIDATION_LAYER)

        enabledLayers.push_back("VK_LAYER_KHRONOS_validation");

    #endif

    return enabledLayers;
}

std::vector<const char *> VulkanGraphicsDriver::GetEnabledDeviceExtensions()
{
    std::vector<const char *> enabledExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    return enabledExtensions;
}

std::vector<const char *> VulkanGraphicsDriver::GetRequiredInstanceExtensions()
{
    SDL_bool sdlResult;
    
    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

    DuskLogVerbose("Available Vulkan Extensions:");
    for (const auto& extension : availableExtensions) {
        DuskLogVerbose("\t%s", extension.extensionName);
    }

    uint32_t requiredExtensionCount;
    SDL_Vulkan_GetInstanceExtensions(GetSDL2Window(), &requiredExtensionCount, nullptr);

    std::vector<const char *> requiredExtensions(requiredExtensionCount);
    sdlResult = SDL_Vulkan_GetInstanceExtensions(GetSDL2Window(), &requiredExtensionCount, requiredExtensions.data());
    if (!sdlResult) {
        DuskLogError("SDL_Vulkan_GetInstanceExtensions() failed, %s", SDL_GetError());
        return std::vector<const char *>();
    }

    #if defined(DUSK_VULKAN_VALIDATION_LAYER)

        bool debugUtilsExtensionFound = false;
        for (const auto& extension : availableExtensions) {
            if (std::string(extension.extensionName) == VK_EXT_DEBUG_UTILS_EXTENSION_NAME) {
                debugUtilsExtensionFound = true;
                break;
            }
        }

        if (debugUtilsExtensionFound) {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        else {
            DuskLogWarn("Vulkan Extension '%s' not available", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

    #endif

    DuskLogVerbose("Required Vulkan Extensions:");
    for (uint32_t i = 0; i < requiredExtensionCount; ++i) {
        DuskLogVerbose("\t%s", requiredExtensions[i]);
    }

    return requiredExtensions;
}

bool VulkanGraphicsDriver::InitInstance()
{
    VkResult vkResult;

    const auto& engineVersion = GetVersion();
    const auto& appVersion = GetApplicationVersion();
    const auto& appName = GetApplicationName();

    const auto& requiredExtensions = GetRequiredInstanceExtensions();
    if (requiredExtensions.empty()) {
        DuskLogError("Failed to get Required Instance Expansions");
        return false;
    }

    VkApplicationInfo applicationInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = appName.c_str(),
        .applicationVersion = VK_MAKE_VERSION(
            appVersion.Major,
            appVersion.Minor,
            appVersion.Patch
        ),
        .pEngineName = "Dusk",
        .engineVersion = VK_MAKE_VERSION(
            engineVersion.Major,
            engineVersion.Minor,
            engineVersion.Patch
        ),
        .apiVersion = VK_API_VERSION_1_1,
    };

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
        .ppEnabledExtensionNames = requiredExtensions.data(),
    };

    vkResult = vkCreateInstance(&createInfo, nullptr, &_vkInstance);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("vkCreateInstance() failed");
        return false;
    }

    // Reload glad to load instance pointers and populate available extensions
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, nullptr, nullptr);
    if (vkVersion == 0) {
        DuskLogError("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance");
        return false;
    }

    return true;
}

void VulkanGraphicsDriver::TermInstance()
{
    vkDestroyInstance(_vkInstance, nullptr);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL _VulkanDebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT * callbackData,
    void * userData)
{
    if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) > 0) {
        Log(LogLevel::Performance, "[PERF](VkDebugUtilsMessenger) %s\n", callbackData->pMessage);
    }
    else {
        switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            Log(LogLevel::Info, "[INFO](VkDebugUtilsMessenger) %s\n", callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            Log(LogLevel::Warning, "[WARN](VkDebugUtilsMessenger) %s\n", callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            Log(LogLevel::Error, "[ERRO](VkDebugUtilsMessenger) %s\n", callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            Log(LogLevel::Verbose, "[VERB](VkDebugUtilsMessenger) %s\n", callbackData->pMessage);
            break;
        }
    }

    return VK_FALSE;
}

bool VulkanGraphicsDriver::InitDebugMessenger()
{
#if !defined(DUSK_VULKAN_VALIDATION_LAYER)
    return true;
#endif

    VkResult vkResult;

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> layers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    bool validationLayerFound = false;
    for (const auto& layer : layers) {
        if (std::string(layer.layerName) == "VK_LAYER_KHRONOS_validation") {
            validationLayerFound = true;
            break;
        }
    }

    if (!validationLayerFound) {
        return false;
    }

    VkDebugUtilsMessageSeverityFlagsEXT messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

    #if defined(DUSK_ENABLE_VERBOSE_LOGGING)
        messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    #endif

    VkDebugUtilsMessageTypeFlagsEXT messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .messageSeverity = messageSeverity,
        .messageType = messageType,
        .pfnUserCallback = _VulkanDebugMessageCallback,
        .pUserData = nullptr,
    };

    if (!vkCreateDebugUtilsMessengerEXT) {
        DuskLogWarn("vkCreateDebugUtilsMessengerEXT() is not bound");
        return false;
    }

    vkResult = vkCreateDebugUtilsMessengerEXT(_vkInstance, &debugUtilsMessengerCreateInfo, nullptr, &_vkDebugMessenger);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("vkCreateDebugUtilsMessengerEXT() failed");
        return false;
    }

    _debugMessengerInitialized = true;

    return true;
}

void VulkanGraphicsDriver::TermDebugMessenger()
{
    if (!_debugMessengerInitialized) {
        return;
    }
    
    if (!vkDestroyDebugUtilsMessengerEXT) {
        DuskLogWarn("vkDestroyDebugUtilsMessengerEXT() is not bound");
        return;
    }

    vkDestroyDebugUtilsMessengerEXT(_vkInstance, _vkDebugMessenger, nullptr);
    _debugMessengerInitialized = false;
}

bool VulkanGraphicsDriver::InitSurface()
{
    SDL_bool sdlResult;

    sdlResult = SDL_Vulkan_CreateSurface(GetSDL2Window(), _vkInstance, &_vkSurface);
    if (!sdlResult) {
        DuskLogError("SDL_Vulkan_CreateSurface() failed, %s", SDL_GetError());
        return false;
    }

    return true;
}

void VulkanGraphicsDriver::TermSurface()
{
    vkDestroySurfaceKHR(_vkInstance, _vkSurface, nullptr);
}

bool VulkanGraphicsDriver::InitPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        DuskLogError("vkEnumeratePhysicalDevices() failed, no devices found");
        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            _vkPhysicalDevice = device;
        }
    }

    if (_vkPhysicalDevice == VK_NULL_HANDLE) {
        DuskLogError("No suitable graphics device found");
        return false;
    }

    DuskLogVerbose("Physical Device Name: %s", _vkPhysicalDeviceProperties.deviceName);

    // Reload glad to load instance pointers and populate available extensions
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, nullptr);
    if (vkVersion == 0) {
        DuskLogError("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance and VkPhysicalDevice");
        return false;
    }

    return true;
}

bool VulkanGraphicsDriver::InitLogicalDevice()
{
    VkResult vkResult;

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, nullptr);
    if (queueFamilyCount == 0) {
        DuskLogError("vkGetPhysicalDeviceQueueFamilyProperties(), no queues found");
        return false;
    }

    std::vector<VkQueueFamilyProperties> queueFamilyProps(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, queueFamilyProps.data());

    _vkGraphicsQueueFamilyIndex = UINT32_MAX;
    _vkPresentQueueFamilyIndex = UINT32_MAX;
    
    for (const auto& prop : queueFamilyProps) {
        std::string types;
        if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            types += "Graphics ";
        }
        if (prop.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            types += "Compute ";
        }
        if (prop.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            types += "Transfer ";
        }
        if (prop.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
            types += "SparseBinding ";
        }

        DuskLogVerbose("Queue Number: %d", prop.queueCount);
        DuskLogVerbose("Queue Flags: %s", types);
    }

    uint32_t index = 0;
    for (const auto& prop : queueFamilyProps) {
        if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            _vkGraphicsQueueFamilyIndex = index;
        }

        VkBool32 presentSupported = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(_vkPhysicalDevice, index, _vkSurface, &presentSupported);

        if (presentSupported) {
            _vkPresentQueueFamilyIndex = index;
        }

        if (_vkGraphicsQueueFamilyIndex != UINT32_MAX && _vkPresentQueueFamilyIndex != UINT32_MAX) {
            break;
        }

        ++index;
    }

    if (_vkGraphicsQueueFamilyIndex == UINT32_MAX) {
        DuskLogError("No suitable graphics queue found");
        return false;
    }

    if (_vkPresentQueueFamilyIndex == UINT32_MAX) {
        DuskLogError("No suitable graphics queue found");
        return false;
    }

    const float queuePriorities = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfoList;
    std::set<uint32_t> uniqueQueueFamilies = { _vkGraphicsQueueFamilyIndex, _vkPresentQueueFamilyIndex };

    for (auto family : uniqueQueueFamilies) {
        queueCreateInfoList.push_back({
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = family,
            .queueCount = 1,
            .pQueuePriorities = &queuePriorities,
        });
    }

    VkPhysicalDeviceFeatures deviceFeatures = {
        // TODO
    };

    const auto& enabledLayers = GetEnabledDeviceLayers();
    const auto& enabledExtensions = GetEnabledDeviceExtensions();

    VkDeviceCreateInfo deviceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfoList.size()),
        .pQueueCreateInfos = queueCreateInfoList.data(),
        .enabledLayerCount = static_cast<uint32_t>(enabledLayers.size()),
        .ppEnabledLayerNames = enabledLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size()),
        .ppEnabledExtensionNames = enabledExtensions.data(),
        .pEnabledFeatures = &deviceFeatures,
    };

    vkResult = vkCreateDevice(_vkPhysicalDevice, &deviceCreateInfo, nullptr, &_vkDevice);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("vkCreateDevice() failed");
        return false;
    }

    // Reload glad to load instance pointers and populate available extensions
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, _vkDevice);
    if (vkVersion == 0) {
        DuskLogError("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance, VkPhysicalDevice, and VkDevice");
        return false;
    }

    vkGetDeviceQueue(_vkDevice, _vkGraphicsQueueFamilyIndex, 0, &_vkGraphicsQueue);
    vkGetDeviceQueue(_vkDevice, _vkPresentQueueFamilyIndex, 0, &_vkPresentQueue);

    return true;
}

void VulkanGraphicsDriver::TermLogicalDevice()
{
    vkDestroyDevice(_vkDevice, nullptr);
}

bool VulkanGraphicsDriver::InitSwapChain()
{
    VkResult vkResult;

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_vkPhysicalDevice, _vkSurface, &surfaceCapabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_vkPhysicalDevice, _vkSurface, &formatCount, nullptr);

    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(_vkPhysicalDevice, _vkSurface, &formatCount, formats.data());

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(_vkPhysicalDevice, _vkSurface, &presentModeCount, nullptr);

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(_vkPhysicalDevice, _vkSurface, &presentModeCount, presentModes.data());

    _vkSwapChainImageFormat = formats[0];
    for (const auto& format : formats) {
        // TODO: Investigate
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            _vkSwapChainImageFormat = format;
            break;
        }
    }

    // TODO: Investigate
    // VK_PRESENT_MODE_FIFO_KHR = Queue of presentation requests, wait for vsync, required to be supported
    // VK_PRESENT_MODE_MAILBOX_KHR = Queue of presentation requests, wait for vsync, replaces entries if the queue is full
    VkPresentModeKHR swapChainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (const auto& presentMode : presentModes) {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            swapChainPresentMode = presentMode;
        }
    }

    // TODO: Remove
    swapChainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

    _vkSwapChainExtent = surfaceCapabilities.currentExtent;
    if (_vkSwapChainExtent.width == UINT32_MAX) {
        ivec2 size = GetWindowSize();

        _vkSwapChainExtent.width = std::clamp(static_cast<uint32_t>(size.x),
            surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);

        _vkSwapChainExtent.height = std::clamp(static_cast<uint32_t>(size.y),
            surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
    }

    // TODO: Investigate
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = _vkSurface,
        .minImageCount = imageCount,
        .imageFormat = _vkSwapChainImageFormat.format,
        .imageColorSpace = _vkSwapChainImageFormat.colorSpace,
        .imageExtent = _vkSwapChainExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = swapChainPresentMode,
        .clipped = VK_TRUE,
    };

    uint32_t queueFamilyIndices[] = {
        _vkGraphicsQueueFamilyIndex,
        _vkPresentQueueFamilyIndex
    };

    // TODO: Refactor
    if (_vkGraphicsQueueFamilyIndex != _vkPresentQueueFamilyIndex) {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    vkResult = vkCreateSwapchainKHR(_vkDevice, &swapChainCreateInfo, nullptr, &_vkSwapChain);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("vkCreateSwapchainKHR() failed");
        return false;
    }

    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &imageCount, nullptr);

    _vkSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &imageCount, _vkSwapChainImages.data());

    _vkSwapChainImageViews.resize(imageCount);

    for (size_t i = 0; i < _vkSwapChainImages.size(); ++i) {
        VkImageViewCreateInfo imageViewCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = _vkSwapChainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = _vkSwapChainImageFormat.format,
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
        };

        vkResult = vkCreateImageView(_vkDevice, &imageViewCreateInfo, nullptr, &_vkSwapChainImageViews[i]);
        if (vkResult != VK_SUCCESS) {
            DuskLogError("vkCreateImageView() failed");
            return false;
        }
    }

    _vkUniformBuffers.resize(_vkSwapChainImages.size());
    _vkUniformBuffersMemory.resize(_vkSwapChainImages.size());

    for (size_t i = 0; i < _vkSwapChainImages.size(); ++i) {
        CreateBuffer(_vkUniformBuffers[i], _vkUniformBuffersMemory[i], 
            sizeof(TransformData), 
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    return true;
}

void VulkanGraphicsDriver::TermSwapChain()
{
    // TODO: Move
    vkDestroyImageView(_vkDevice, _vkDepthImageView, nullptr);
    vkDestroyImage(_vkDevice, _vkDepthImage, nullptr);
    vkFreeMemory(_vkDevice, _vkDepthImageMemory, nullptr);

    // TODO: Move
    for (const auto& framebuffer : _vkFramebuffers) {
        vkDestroyFramebuffer(_vkDevice, framebuffer, nullptr);
    }

    // TODO: Move
    vkFreeCommandBuffers(_vkDevice, _vkCommandPool, static_cast<size_t>(_vkCommandBuffers.size()), _vkCommandBuffers.data());

    for (const auto& pipeline : _pipelines) {
        pipeline->Terminate();
    }

    vkDestroyPipelineLayout(_vkDevice, _vkPipelineLayout, nullptr);
    vkDestroyRenderPass(_vkDevice, _vkRenderPass, nullptr);

    for (const auto& imageView : _vkSwapChainImageViews) {
        vkDestroyImageView(_vkDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(_vkDevice, _vkSwapChain, nullptr);

    for (size_t i = 0; i < _vkSwapChainImages.size(); i++) {
        vkDestroyBuffer(_vkDevice, _vkUniformBuffers[i], nullptr);
        vkFreeMemory(_vkDevice, _vkUniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorPool(_vkDevice, _vkDescriptorPool, nullptr);
}

bool VulkanGraphicsDriver::ResetSwapChain()
{
    DuskBenchmarkStart();

    vkDeviceWaitIdle(_vkDevice);

    TermSwapChain();

    if (!InitSwapChain()) {
        return false;
    }

    if (!InitRenderPass()) {
        return false;
    }

    if (!InitDescriptorPool()) {
        return false;
    }

    if (!InitGraphicsPipelines()) {
        return false;
    }

    if (!InitDepthBuffer()) {
        return false;
    }

    if (!InitFramebuffers()) {
        return false;
    }

    if (!InitCommandPool()) {
        return false;
    }

    if (!InitCommandBuffers()) {
        return false;
    }

    DuskBenchmarkEnd("VulkanGraphicsDriver::ResetSwapChain()");
    return true;
}

bool VulkanGraphicsDriver::InitRenderPass()
{
    VkResult vkResult;

    VkAttachmentDescription colorAttachmentDescription = {
        .flags = 0,
        .format = _vkSwapChainImageFormat.format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    VkAttachmentDescription depthAttachmentDescription = {
        .flags = 0,
        .format = _vkDepthImageFormat,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    VkAttachmentReference colorAttachmentReference = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkAttachmentReference detphAttachmentReference = {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpassDescription = {
        .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentReference,
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = &detphAttachmentReference,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = nullptr,
    };

    VkSubpassDependency subpassDependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    };

    std::array<VkAttachmentDescription, 2> attachments = {
        colorAttachmentDescription,
        depthAttachmentDescription,
    };

    VkRenderPassCreateInfo renderPassCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .subpassCount = 1,
        .pSubpasses = &subpassDescription,
        .dependencyCount = 1,
        .pDependencies = &subpassDependency,
    };

    vkResult = vkCreateRenderPass(_vkDevice, &renderPassCreateInfo, nullptr, &_vkRenderPass);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("vkCreateRenderPass() failed");
        return false;
    }

    return true;
}

bool VulkanGraphicsDriver::InitDescriptorPool()
{
    VkResult vkResult;

    VkDescriptorPoolSize descriptorPoolSize = {
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = static_cast<uint32_t>(_vkSwapChainImages.size()),
    };

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = static_cast<uint32_t>(_vkSwapChainImages.size()),
        .poolSizeCount = 1,
        .pPoolSizes = &descriptorPoolSize,
    };

    vkResult = vkCreateDescriptorPool(_vkDevice, &descriptorPoolCreateInfo, nullptr, &_vkDescriptorPool);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("vkCreateDescriptorPool() failed");
        return false;
    }

    // TODO: Move
    VkDescriptorSetLayoutBinding layoutBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr,
    };

    VkDescriptorSetLayoutCreateInfo descriptorLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .bindingCount = 1,
        .pBindings = &layoutBinding,
    };

    vkResult = vkCreateDescriptorSetLayout(_vkDevice, &descriptorLayoutCreateInfo, nullptr, &_vkDescriptorSetLayout);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("vkCreateDescriptorSetLayout() failed");
        return false;
    }

    VkDescriptorSetLayout setLayouts[] = { _vkDescriptorSetLayout };

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = 1,
        .pSetLayouts = setLayouts,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    vkResult = vkCreatePipelineLayout(_vkDevice, &pipelineLayoutCreateInfo, nullptr, &_vkPipelineLayout);
    if (vkResult != VK_SUCCESS) {
        DuskLogFatal("vkCreatePipelineLayout() failed");
    }


    std::vector<VkDescriptorSetLayout> layouts(_vkSwapChainImages.size(), _vkDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = _vkDescriptorPool,
        .descriptorSetCount = static_cast<uint32_t>(_vkSwapChainImages.size()),
        .pSetLayouts = layouts.data(),
    };

    _vkDescriptorSets.resize(_vkSwapChainImages.size());
    vkResult = vkAllocateDescriptorSets(_vkDevice, &allocateInfo, _vkDescriptorSets.data());
    if (vkResult != VK_SUCCESS) {
        DuskLogError("vkAllocateDescriptorSets() failed");
        return false;
    }

    for (size_t i = 0; i < _vkSwapChainImages.size(); ++i) {
        VkDescriptorBufferInfo bufferInfo = {
            .buffer = _vkUniformBuffers[i],
            .offset = 0,
            .range = sizeof(TransformData),
        };

        VkWriteDescriptorSet writeDescriptorSet = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = _vkDescriptorSets[i],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &bufferInfo,
        };

        vkUpdateDescriptorSets(_vkDevice, 1, &writeDescriptorSet, 0, nullptr);
    }

    return true;
}

bool VulkanGraphicsDriver::InitGraphicsPipelines()
{
    for (auto& pipeline : _pipelines) {
        if (!pipeline->Initialize()) {
            return false;
        }
    }

    return true;
}

bool VulkanGraphicsDriver::InitFramebuffers()
{
    VkResult vkResult;

    _vkFramebuffers.resize(_vkSwapChainImageViews.size());

    for (size_t i = 0; i < _vkSwapChainImageViews.size(); ++i) {
        std::array<VkImageView, 2> attachments = {
            _vkSwapChainImageViews[i],
            _vkDepthImageView,
        };

        VkFramebufferCreateInfo framebufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .renderPass = _vkRenderPass,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .width = _vkSwapChainExtent.width,
            .height = _vkSwapChainExtent.height,
            .layers = 1,
        };

        vkResult = vkCreateFramebuffer(_vkDevice, &framebufferCreateInfo, nullptr, &_vkFramebuffers[i]);
        if (vkResult != VK_SUCCESS) {
            DuskLogError("vkCreateFramebuffer() failed");
            return false;
        }
    }

    return true;
}

bool VulkanGraphicsDriver::InitCommandPool()
{
    VkResult vkResult;

    VkCommandPoolCreateInfo commandPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = _vkGraphicsQueueFamilyIndex,
    };

    vkResult = vkCreateCommandPool(_vkDevice, &commandPoolCreateInfo, nullptr, &_vkCommandPool);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("vkCreateCommandPool() failed");
        return false;
    }

    return true;
}

bool VulkanGraphicsDriver::InitDepthBuffer()
{
    std::vector<VkFormat> potentialDepthFormats = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
    };

    _vkDepthImageFormat = VK_FORMAT_UNDEFINED;

    for (VkFormat format : potentialDepthFormats) {
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(_vkPhysicalDevice, format, &formatProperties);

        VkFormatFeatureFlags features = (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        if (features == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            _vkDepthImageFormat = format;
            break;
        }
    }

    if (_vkDepthImageFormat == VK_FORMAT_UNDEFINED) {
        DuskLogError("Failed to find suitable depth buffer image format");
        return false;
    }

    VkImageCreateInfo imageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = _vkDepthImageFormat,
        .extent = {
            .width = _vkSwapChainExtent.width,
            .height = _vkSwapChainExtent.height,
            .depth = 1,
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    if (vkCreateImage(_vkDevice, &imageCreateInfo, nullptr, &_vkDepthImage) != VK_SUCCESS) {
        DuskLogError("Failed to create depth buffer image");
        return false;
    }

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(_vkDevice, _vkDepthImage, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
    };

    if (vkAllocateMemory(_vkDevice, &memoryAllocateInfo, nullptr, &_vkDepthImageMemory) != VK_SUCCESS) {
        DuskLogError("Failed to allocate depth buffer image memory");
        return false;
    }

    vkBindImageMemory(_vkDevice, _vkDepthImage, _vkDepthImageMemory, 0);

    VkImageViewCreateInfo imageViewCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = _vkDepthImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_D32_SFLOAT,
        // .components
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    if (vkCreateImageView(_vkDevice, &imageViewCreateInfo, nullptr, &_vkDepthImageView) != VK_SUCCESS) {
        DuskLogError("Failed to create depth buffer image view");
        return false;
    }

    return true;
}

bool VulkanGraphicsDriver::InitCommandBuffers()
{
    VkResult vkResult;

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = _vkCommandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = static_cast<uint32_t>(_vkFramebuffers.size()),
    };

    _vkCommandBuffers.resize(_vkFramebuffers.size());

    vkResult = vkAllocateCommandBuffers(_vkDevice, &commandBufferAllocateInfo, _vkCommandBuffers.data());
    if (vkResult != VK_SUCCESS) {
        DuskLogError("vkAllocateCommandBuffers() failed");
        return false;
    }

    for (size_t i = 0; i < _vkCommandBuffers.size(); ++i) {
        VkCommandBufferBeginInfo commandBufferBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr,
        };

        vkResult = vkBeginCommandBuffer(_vkCommandBuffers[i], &commandBufferBeginInfo);
        if (vkResult != VK_SUCCESS) {
            DuskLogError("vkBeginCommandBuffer() failed");
            return false;
        }

        std::array<VkClearValue, 2> clearValues = { };

        const vec4& cc = GetClearColor();
        clearValues[0].color = {
            .float32 = { cc.r, cc.g, cc.b, cc.a }
        };
        
        clearValues[1].depthStencil = { 
            .depth = 1.0f, 
            .stencil = 0,
        };

        VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = _vkRenderPass,
            .framebuffer = _vkFramebuffers[i],
            .renderArea = {
                .offset = { 0, 0 },
                .extent = _vkSwapChainExtent,
            },
            .clearValueCount = static_cast<uint32_t>(clearValues.size()),
            .pClearValues = clearValues.data(),
        };

        vkCmdBeginRenderPass(_vkCommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        for (const auto& p : _pipelines) {
            VulkanPipeline * pipeline = DUSK_VULKAN_PIPELINE(p.get());
            
            // pipeline->GenerateBindCommands(_vkCommandBuffers[i]);
            
            vkCmdBindDescriptorSets(_vkCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _vkPipelineLayout, 0, 1, &_vkDescriptorSets[i], 0, nullptr);
            
            // pipeline->GenerateDrawCommands(_vkCommandBuffers[i]);
        }

        vkCmdEndRenderPass(_vkCommandBuffers[i]);

        vkResult = vkEndCommandBuffer(_vkCommandBuffers[i]);
        if (vkResult != VK_SUCCESS) {
            DuskLogError("vkEndCommandBuffer() failed");
            return false;
        }
    }

    return true;
}

bool VulkanGraphicsDriver::InitSyncObjects()
{
    VkResult vkResult;

    _vkImageAvailableSemaphores.resize(2);
    _vkRenderingFinishedSemaphores.resize(2);
    _vkInFlightFences.resize(2);
    _vkImagesInFlight.resize(_vkSwapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
    };

    VkFenceCreateInfo fenceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    for (size_t i = 0; i < 2; ++i) {
        vkResult = vkCreateSemaphore(_vkDevice, &semaphoreCreateInfo, nullptr, &_vkImageAvailableSemaphores[i]);
        if (vkResult != VK_SUCCESS) {
            DuskLogError("vkCreateSemaphore() failed");
            return false;
        }
        else {
            vkResult = vkCreateSemaphore(_vkDevice, &semaphoreCreateInfo, nullptr, &_vkRenderingFinishedSemaphores[i]);
            if (vkResult != VK_SUCCESS) {
                DuskLogError("vkCreateSemaphore() failed");
                return false;
            }
            else {
                vkResult = vkCreateFence(_vkDevice, &fenceCreateInfo, nullptr, &_vkInFlightFences[i]);
                if (vkResult != VK_SUCCESS) {
                    DuskLogError("vkCreateFence() failed");
                    return false;
                }   
            }
        }
    }

    return true;
}

// bool VulkanGraphicsDriver::InitGraphicsPipeline()
// {
//     _shader = CreateShader();
//     _shader->LoadFromFiles({
//         "flat.vert",
//         "flat.frag",
//     });
    
//     _mesh = CreateMesh();
//     const auto& meshImporters = GetAllMeshImporters();
//     auto meshDatas = meshImporters[0]->LoadFromFile("crate/crate.obj");
//     if (meshDatas.empty()) {
//         DuskLogFatal("Failed to init hacky code in InitGraphicsPipeline()");
//     }
//     _mesh->Load(meshDatas[0].get());

//     _pipeline = CreatePipeline();

//     _pipeline->SetCullMode(CullMode::Back);
//     _pipeline->SetFrontFace(FrontFace::CounterClockwise);
//     _pipeline->SetPrimitiveTopology(PrimitiveTopology::Triangles);

//     _pipeline->SetShader(_shader.get());
//     _pipeline->SetMesh(_mesh.get());
//     _pipeline->Create();

//     return true;
// }

// void VulkanGraphicsDriver::TermGraphicsPipeline()
// {
//     _pipeline = nullptr;
//     _mesh = nullptr;
//     _shader = nullptr;
// }

} // namespace Dusk::Vulkan
