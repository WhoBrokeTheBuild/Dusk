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
    
    if (!InitWindow() ||
        !InitInstance()) {
        return false;
    }

    InitDebugMessenger();

    if (!InitSurface() ||
        !InitPhysicalDevice() ||
        !InitLogicalDevice() ||
        !InitSwapChain() ||
        !InitRenderPass() ||
        !InitDescriptorPool() ||
        !InitGraphicsPipeline() ||
        !InitFramebuffers() ||
        !InitCommandPool() ||
        !InitCommandBuffers() ||
        !InitSyncObjects()) {
        return false;
    }

    DuskBenchmarkEnd("VulkanGraphicsDriver::Initialize");
    return true;
}

DUSK_VULKAN_API
void VulkanGraphicsDriver::Terminate()
{
    DuskBenchmarkStart();

    _shader = nullptr;
    _mesh = nullptr;
    _pipeline = nullptr;

    vkDeviceWaitIdle(_vkDevice);

    TermCommandPool();
    TermFramebuffers();
    TermGraphicsPipeline();
    TermRenderPass();
    TermSwapChain();
    TermLogicalDevice();
    TermSurface();
    TermDebugMessenger();
    TermInstance();
    TermWindow();

    DuskBenchmarkEnd("VulkanGraphicsDriver::Terminate");
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
void VulkanGraphicsDriver::SwapBuffers()
{
    VkResult vkResult;

    vkWaitForFences(_vkDevice, 1, &_vkInFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex = 0;
    vkResult = vkAcquireNextImageKHR(_vkDevice, _vkSwapChain, UINT64_MAX, _vkImageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (vkResult == VK_ERROR_OUT_OF_DATE_KHR) {
        if (!ResizeSwapChain()) {
            DuskLogFatal("Failed to resize swap chain");
        }
    }
    else if (vkResult != VK_SUCCESS && vkResult != VK_SUBOPTIMAL_KHR) {
        DuskLogFatal("vkAcquireNextImageKHR() failed");
    }


    Camera camera;
    camera.SetPosition({ 10, 10, 10 });
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
        if (!ResizeSwapChain()) {
            DuskLogFatal("Failed to resize swap chain");
        }
    }
    else if (vkResult != VK_SUCCESS) {
        DuskLogFatal("vkQueuePresentKHR() failed");
    }

    _currentFrame = (_currentFrame + 1) % 2;
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
    SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &requiredExtensionCount, nullptr);

    std::vector<const char *> requiredExtensions(requiredExtensionCount);
    sdlResult = SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &requiredExtensionCount, requiredExtensions.data());
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

bool VulkanGraphicsDriver::InitWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        DuskLogError("SDL_Init() failed, %s", SDL_GetError());
        return false;
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

    int vkVersion = gladLoaderLoadVulkan(nullptr, nullptr, nullptr);
    if (vkVersion == 0) {
        DuskLogFatal("gladLoaderLoadVulkan() failed");
    }
    
    DuskLogVerbose("Vulkan %d.%d", GLAD_VERSION_MAJOR(vkVersion), GLAD_VERSION_MINOR(vkVersion));

    return true; 
}

void VulkanGraphicsDriver::TermWindow()
{
    SDL_DestroyWindow(_sdlWindow);
    SDL_Quit();
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

    sdlResult = SDL_Vulkan_CreateSurface(_sdlWindow, _vkInstance, &_vkSurface);
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
    for (size_t i = 0; i < _vkSwapChainImages.size(); i++) {
        vkDestroyBuffer(_vkDevice, _vkUniformBuffers[i], nullptr);
        vkFreeMemory(_vkDevice, _vkUniformBuffersMemory[i], nullptr);
    }

    for (const auto& imageView : _vkSwapChainImageViews) {
        vkDestroyImageView(_vkDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(_vkDevice, _vkSwapChain, nullptr);
}

bool VulkanGraphicsDriver::ResizeSwapChain()
{
    vkDeviceWaitIdle(_vkDevice);

    TermSwapChain();

    if (!InitSwapChain() ||
        !InitRenderPass() ||
        !InitDescriptorPool() ||
        !InitGraphicsPipeline() ||
        !InitFramebuffers() ||
        !InitCommandPool() ||
        !InitCommandBuffers()) {
        return false;
    }

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
        DuskLogError("vkCreateRenderPass() failed");
        return false;
    }

    return true;
}

void VulkanGraphicsDriver::TermRenderPass()
{
    vkDestroyRenderPass(_vkDevice, _vkRenderPass, nullptr);
}

bool VulkanGraphicsDriver::InitFramebuffers()
{
    VkResult vkResult;

    _vkFramebuffers.resize(_vkSwapChainImageViews.size());

    for (size_t i = 0; i < _vkSwapChainImageViews.size(); ++i) {
        VkImageView attachments[] = {
            _vkSwapChainImageViews[i],
        };

        VkFramebufferCreateInfo framebufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .renderPass = _vkRenderPass,
            .attachmentCount = 1,
            .pAttachments = attachments,
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

void VulkanGraphicsDriver::TermFramebuffers()
{
    for (const auto& framebuffer : _vkFramebuffers) {
        vkDestroyFramebuffer(_vkDevice, framebuffer, nullptr);
    }
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

void VulkanGraphicsDriver::TermCommandPool()
{
    vkDestroyCommandPool(_vkDevice, _vkCommandPool, nullptr);
}

bool VulkanGraphicsDriver::InitCommandBuffers()
{
    VkResult vkResult;

    VulkanPipeline * pipeline = DUSK_VULKAN_PIPELINE(_pipeline.get());

    _vkCommandBuffers.resize(_vkFramebuffers.size());

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = _vkCommandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = static_cast<uint32_t>(_vkFramebuffers.size()),
    };

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

        VkClearValue clearColor = { 
            .color = {{ 0.392f, 0.584f, 0.929f, 1.0f }},
        };

        VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = _vkRenderPass,
            .framebuffer = _vkFramebuffers[i],
            .renderArea = {
                .offset = { 0, 0 },
                .extent = _vkSwapChainExtent,
            },
            .clearValueCount = 1,
            .pClearValues = &clearColor,
        };

        // TODO: Refactor/Move
        vkCmdBeginRenderPass(_vkCommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(_vkCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetVkPipeline());

        VulkanMesh * mesh = DUSK_VULKAN_MESH(_mesh.get());
        const auto& buffers = mesh->GetVkBuffers();
        VkDeviceSize offsets[] = { 0 };

        vkCmdBindVertexBuffers(_vkCommandBuffers[i], 0, static_cast<uint32_t>(buffers.size()), buffers.data(), offsets);

        vkCmdBindDescriptorSets(_vkCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _vkPipelineLayout, 0, 1, &_vkDescriptorSets[i], 0, nullptr);

        // Triangles = / 3
        vkCmdDraw(_vkCommandBuffers[i], mesh->GetVertexCount() / 3, 1, 0, 0);

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

bool VulkanGraphicsDriver::InitGraphicsPipeline()
{
    _shader = CreateShader();
    _shader->LoadFromFiles({
        "flat.vert",
        "flat.frag",
    });
    
    _mesh = CreateMesh();
    const auto& meshImporters = GetAllMeshImporters();
    auto meshDatas = meshImporters[0]->LoadFromFile("crate/crate.obj");
    if (meshDatas.empty()) {
        DuskLogFatal("Failed to init hacky code in InitGraphicsPipeline()");
    }
    _mesh->Load(meshDatas[0].get());

    _pipeline = CreatePipeline();

    _pipeline->SetCullMode(CullMode::Back);
    _pipeline->SetFrontFace(FrontFace::CounterClockwise);
    _pipeline->SetPrimitiveTopology(PrimitiveTopology::Triangles);

    _pipeline->SetShader(_shader.get());
    _pipeline->SetMesh(_mesh.get());
    _pipeline->Create();

    return true;
}

void VulkanGraphicsDriver::TermGraphicsPipeline()
{
    _pipeline = nullptr;
    _mesh = nullptr;
    _shader = nullptr;
}

} // namespace Dusk::Vulkan
