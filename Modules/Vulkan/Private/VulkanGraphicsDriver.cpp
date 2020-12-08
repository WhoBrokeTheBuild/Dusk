#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>

#include <set>

namespace Dusk {

DUSK_VULKAN_API
VulkanGraphicsDriver::VulkanGraphicsDriver()
{
    InitWindow();
    InitInstance();
    InitDebugMessenger();
    InitSurface();
    InitPhysicalDevice();
    InitLogicalDevice();
    InitSwapChain();
    InitRenderPass();
} 

DUSK_VULKAN_API
VulkanGraphicsDriver::~VulkanGraphicsDriver()
{
    TermRenderPass();
    TermSwapChain();
    TermLogicalDevice();
    TermSurface();
    TermDebugMessenger();
    TermInstance();
    TermWindow();
}

DUSK_VULKAN_API
void VulkanGraphicsDriver::SetWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(_sdlWindow, title.c_str());
}

DUSK_VULKAN_API
std::string VulkanGraphicsDriver::GetWindowTitle()
{
    return SDL_GetWindowTitle(_sdlWindow);
}

DUSK_VULKAN_API
void VulkanGraphicsDriver::SetWindowSize(const ivec2& size)
{
    SDL_SetWindowSize(_sdlWindow, size.x, size.y);
    // glViewport(0, 0, size.x, size.y);
}

DUSK_VULKAN_API
ivec2 VulkanGraphicsDriver::GetWindowSize()
{
    ivec2 size;
    SDL_GetWindowSize(_sdlWindow, &size.x, &size.y);
    return size;
}

DUSK_VULKAN_API
void VulkanGraphicsDriver::ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0) {
        if (event.type == SDL_QUIT) {
            SetRunning(false);
        }
    }
}

DUSK_VULKAN_API
void VulkanGraphicsDriver::SwapBuffers() {
    // glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(_sdlWindow);
}

DUSK_VULKAN_API
std::shared_ptr<Pipeline> VulkanGraphicsDriver::CreatePipeline()
{
    return std::shared_ptr<Pipeline>(New VulkanPipeline());
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
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);

    // TODO:
    // * Queue Families
    // * Device Extensions
    // * Swap Chain Support

    return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
        && features.geometryShader;
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
    SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &requiredExtensionCount, nullptr);

    std::vector<const char *> requiredExtensions(requiredExtensionCount);
    sdlResult = SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &requiredExtensionCount, requiredExtensions.data());
    if (!sdlResult) {
        DuskLogFatal("SDL_Vulkan_GetInstanceExtensions() failed, %s", SDL_GetError());
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

void VulkanGraphicsDriver::InitWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        DuskLogFatal("SDL_Init() failed, %s", SDL_GetError());
    }

    SDL_version sdlVersion;
    SDL_GetVersion(&sdlVersion);
    DuskLogVerbose("SDL Version: %d.%d.%d", 
        (int)sdlVersion.major, 
        (int)sdlVersion.minor, 
        (int)sdlVersion.patch);

    std::string title = GetApplicationName() + " (" + GetApplicationVersion().GetString() + ")";

    _sdlWindow = SDL_CreateWindow(title.c_str(), 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480, 
        SDL_WINDOW_VULKAN);

    uint16_t pixels[16 * 16] = { 0xFFFF };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(
        pixels, 16, 16, 16, 16 * 2,
        0x0f00, 0x00f0, 0x000f, 0xf000);

    SDL_SetWindowIcon(_sdlWindow, surface);
    SDL_FreeSurface(surface);

    int vkVersion = gladLoaderLoadVulkan(NULL, NULL, NULL);
    if (vkVersion == 0) {
        DuskLogFatal("gladLoaderLoadVulkan() failed");
    }
    
    DuskLogVerbose("Vulkan %d.%d", GLAD_VERSION_MAJOR(vkVersion), GLAD_VERSION_MINOR(vkVersion));
}

void VulkanGraphicsDriver::TermWindow()
{
    SDL_DestroyWindow(_sdlWindow);
    SDL_Quit();
}

void VulkanGraphicsDriver::InitInstance()
{
    VkResult vkResult;

    const auto& engineVersion = GetVersion();
    const auto& appVersion = GetApplicationVersion();
    const auto& appName = GetApplicationName();

    const auto& requiredExtensions = GetRequiredInstanceExtensions();

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
        DuskLogFatal("vkCreateInstance() failed");
        return;
    }

    // Reload glad to load instance pointers and populate available extensions
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, NULL);
    if (vkVersion == 0) {
        DuskLogFatal("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance and VkPhysicalDevice");
    }
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

void VulkanGraphicsDriver::InitDebugMessenger()
{
#if !defined(DUSK_VULKAN_VALIDATION_LAYER)
    return;
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
        return;
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
        return;
    }

    vkResult = vkCreateDebugUtilsMessengerEXT(_vkInstance, &debugUtilsMessengerCreateInfo, nullptr, &_vkDebugMessenger);
    if (vkResult != VK_SUCCESS) {
        DuskLogFatal("vkCreateDebugUtilsMessengerEXT() failed");
    }

    _debugMessengerInitialized = true;
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

void VulkanGraphicsDriver::InitSurface()
{
    SDL_bool sdlResult;

    sdlResult = SDL_Vulkan_CreateSurface(_sdlWindow, _vkInstance, &_vkWindowSurface);
    if (!sdlResult) {
        DuskLogFatal("SDL_Vulkan_CreateSurface() failed, %s", SDL_GetError());
    }
}

void VulkanGraphicsDriver::TermSurface()
{
    vkDestroySurfaceKHR(_vkInstance, _vkWindowSurface, nullptr);
}

void VulkanGraphicsDriver::InitPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        DuskLogFatal("vkEnumeratePhysicalDevices() failed, no devices found");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            _vkPhysicalDevice = device;
        }
    }

    if (_vkPhysicalDevice == VK_NULL_HANDLE) {
        DuskLogFatal("No suitable graphics device found");
    }
}

void VulkanGraphicsDriver::InitLogicalDevice()
{
    VkResult vkResult;

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, nullptr);
    if (queueFamilyCount == 0) {
        DuskLogFatal("vkGetPhysicalDeviceQueueFamilyProperties(), no queues found");
    }

    std::vector<VkQueueFamilyProperties> queueFamilyProps(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, queueFamilyProps.data());

    _vkGraphicsQueueFamilyIndex = UINT32_MAX;
    _vkPresentQueueFamilyIndex = UINT32_MAX;
    
    uint32_t index = 0;
    for (const auto& prop : queueFamilyProps) {
        if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            _vkGraphicsQueueFamilyIndex = index;
        }

        VkBool32 presentSupported = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(_vkPhysicalDevice, index, _vkWindowSurface, &presentSupported);

        if (presentSupported) {
            _vkPresentQueueFamilyIndex = index;
        }

        if (_vkGraphicsQueueFamilyIndex != UINT32_MAX && _vkPresentQueueFamilyIndex != UINT32_MAX) {
            break;
        }

        ++index;
    }

    if (_vkGraphicsQueueFamilyIndex == UINT32_MAX) {
        DuskLogFatal("No suitable graphics queue found");
    }

    if (_vkPresentQueueFamilyIndex == UINT32_MAX) {
        DuskLogFatal("No suitable graphics queue found");
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
        DuskLogFatal("vkCreateDevice() failed");
    }

    // Reload glad to load instance pointers and populate available extensions
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, _vkDevice);
    if (vkVersion == 0) {
        DuskLogFatal("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance, VkPhysicalDevice, and VkDevice");
    }

    vkGetDeviceQueue(_vkDevice, _vkGraphicsQueueFamilyIndex, 0, &_vkGraphicsQueue);
    vkGetDeviceQueue(_vkDevice, _vkPresentQueueFamilyIndex, 0, &_vkPresentQueue);
}

void VulkanGraphicsDriver::TermLogicalDevice()
{
    vkDestroyDevice(_vkDevice, nullptr);
}

void VulkanGraphicsDriver::InitSwapChain()
{
    VkResult vkResult;

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_vkPhysicalDevice, _vkWindowSurface, &surfaceCapabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_vkPhysicalDevice, _vkWindowSurface, &formatCount, nullptr);

    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(_vkPhysicalDevice, _vkWindowSurface, &formatCount, formats.data());

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(_vkPhysicalDevice, _vkWindowSurface, &presentModeCount, nullptr);

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(_vkPhysicalDevice, _vkWindowSurface, &presentModeCount, presentModes.data());

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
        .surface = _vkWindowSurface,
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
        DuskLogFatal("vkCreateSwapchainKHR failed");
    }

    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &imageCount, nullptr);

    _vkSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &imageCount, _vkSwapChainImages.data());

    _vkSwapChainImageViews.reserve(imageCount);
    for (const auto& image : _vkSwapChainImages) {
        VkImageViewCreateInfo imageViewCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
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

        VkImageView imageView;
        vkResult = vkCreateImageView(_vkDevice, &imageViewCreateInfo, nullptr, &imageView);
        if (vkResult != VK_SUCCESS) {
            DuskLogFatal("vkCreateImageView failed");
        }

        _vkSwapChainImageViews.push_back(imageView);
    }
}

void VulkanGraphicsDriver::TermSwapChain()
{
    for (const auto& imageView : _vkSwapChainImageViews) {
        vkDestroyImageView(_vkDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(_vkDevice, _vkSwapChain, nullptr);
}

void VulkanGraphicsDriver::InitRenderPass()
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

    VkAttachmentReference colorAttachmentReference = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpassDescription = {
        .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentReference,
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = nullptr,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = nullptr,
    };

    VkSubpassDependency subpassDependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    };

    VkRenderPassCreateInfo renderPassCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .attachmentCount = 1,
        .pAttachments = &colorAttachmentDescription,
        .subpassCount = 1,
        .pSubpasses = &subpassDescription,
        .dependencyCount = 1,
        .pDependencies = &subpassDependency,
    };

    vkResult = vkCreateRenderPass(_vkDevice, &renderPassCreateInfo, nullptr, &_vkRenderPass);
    if (vkResult != VK_SUCCESS) {
        DuskLogFatal("vkCreateRenderPass failed");
    }
}

void VulkanGraphicsDriver::TermRenderPass()
{
    vkDestroyRenderPass(_vkDevice, _vkRenderPass, nullptr);
}

} // namespace Dusk
