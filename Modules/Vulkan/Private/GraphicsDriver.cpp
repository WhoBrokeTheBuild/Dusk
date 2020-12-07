#include <Dusk/Vulkan/GraphicsDriver.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Dusk.hpp>

#include <SDL_vulkan.h>

#undef CreateWindow
#undef near
#undef far

namespace Dusk {


DUSK_VULKAN_API
VulkanGraphicsDriver::VulkanGraphicsDriver() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        DuskLogError("Failed to initialize SDL, %s", SDL_GetError());
        return;
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

    Uint16 pixels[16 * 16] = { 0xFFFF };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2,
                                                     0x0f00, 0x00f0, 0x000f, 0xf000);
    SDL_SetWindowIcon(_sdlWindow, surface);
    SDL_FreeSurface(surface);

    int vkVersion = gladLoaderLoadVulkan(NULL, NULL, NULL);
    if (!vkVersion) {
        DuskLogError("Failed to load Vulkan symbols");
        return;
    }
    
    DuskLogVerbose("Vulkan %d.%d", GLAD_VERSION_MAJOR(vkVersion), GLAD_VERSION_MINOR(vkVersion));

    // Extensions

    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

    DuskLogVerbose("Available Vulkan Extensions:");
    for (const auto& extension : availableExtensions) {
        DuskLogVerbose("\t%s", extension.extensionName);
    }

    static const char * requiredExtensions[64];
    uint32_t requiredExtensionCount = sizeof(requiredExtensions) / sizeof(requiredExtensions[0]);

    SDL_bool sdlResult = SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &requiredExtensionCount, requiredExtensions);
    if (!sdlResult) {
        DuskLogError("SDL_Vulkan_GetInstanceExtensions failed, %s", SDL_GetError());
        return;
    }

    DuskLogVerbose("Required Vulkan Extensions:");
    for (uint32_t i = 0; i < requiredExtensionCount; ++i) {
        DuskLogVerbose("\t%s", requiredExtensions[i]);
    }

    const auto& version = GetVersion();
    const auto& appVersion = GetApplicationVersion();
    const auto& appName = GetApplicationName();

    // Instance

    VkApplicationInfo appInfo = { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pEngineName = "Dusk";
    appInfo.engineVersion = VK_MAKE_VERSION(
        version.Major,
        version.Minor,
        version.Patch);
    appInfo.pApplicationName = appName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(
        appVersion.Major,
        appVersion.Minor,
        appVersion.Patch);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = requiredExtensionCount;
    createInfo.ppEnabledExtensionNames = requiredExtensions;

    if (vkCreateInstance(&createInfo, nullptr, &_vkInstance) != VK_SUCCESS) {
        DuskLogError("Failed to create Vulkan instance");
        return;
    }

    sdlResult = SDL_Vulkan_CreateSurface(_sdlWindow, _vkInstance, &_vkWindowSurface);
    if (!sdlResult) {
        DuskLogError("SDL_Vulkan_CreateSurface failed, %s", SDL_GetError());
        return;
    }

    // Physical Devices

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        DuskLogError("Failed to find GPUs with Vulkan Support");
        return;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            _vkPhysicalDevice = device;
        }
    }

    if (_vkPhysicalDevice == VK_NULL_HANDLE) {
        DuskLogError("Failed to find a suitable GPU\n");
        return;
    }

    // Queue

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, nullptr);
    if (queueFamilyCount < 1) {
        DuskLogError("Failed to get Queue Families");
        return;
    }

    std::vector<VkQueueFamilyProperties> queueFamilyProps(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, queueFamilyProps.data());

    VkDeviceQueueCreateInfo queueCreateInfo;
    queueCreateInfo.queueFamilyIndex = 0;
    
    for (const auto& prop : queueFamilyProps) {
        if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            break;
        }
        ++queueCreateInfo.queueFamilyIndex;
    }

    if (queueCreateInfo.queueFamilyIndex == queueFamilyCount) {
        DuskLogError("Failed to get Queue");
        return;
    }
    
    const float queuePriorities = 0.f;
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = nullptr;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriorities;

    // Device

    VkDeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.ppEnabledExtensionNames = nullptr;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledExtensionNames = nullptr;
    deviceCreateInfo.pEnabledFeatures = nullptr;

    VkResult vkResult = vkCreateDevice(_vkPhysicalDevice, &deviceCreateInfo, nullptr, &_vkDevice);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("Failed to create Device");
        return;
    }

    // Command Pool

    VkCommandPoolCreateInfo commandPoolCreateInfo;
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.pNext = nullptr;
    commandPoolCreateInfo.queueFamilyIndex = queueCreateInfo.queueFamilyIndex;
    commandPoolCreateInfo.flags = 0;

    vkResult = vkCreateCommandPool(_vkDevice, &commandPoolCreateInfo, nullptr, &_vkCommandPool);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("Failed to create Command Pool");
        return;
    }

    // Command Buffer

    VkCommandBufferAllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = nullptr;
    commandBufferAllocateInfo.commandPool = _vkCommandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = 1;

    _vkCommandBuffers.resize(1);
    vkResult = vkAllocateCommandBuffers(_vkDevice, &commandBufferAllocateInfo, _vkCommandBuffers.data());
    if (vkResult != VK_SUCCESS) {
        DuskLogError("Failed to create Command Buffers");
        return;
    }

    // Swapchain

    std::vector<VkBool32> supportsPresent(queueFamilyCount);
    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
        vkGetPhysicalDeviceSurfaceSupportKHR(_vkPhysicalDevice, i, _vkWindowSurface, &supportsPresent[i]);
    }





} 

DUSK_VULKAN_API
VulkanGraphicsDriver::~VulkanGraphicsDriver() {
    SDL_DestroyWindow(_sdlWindow);
    SDL_Quit();
}

DUSK_VULKAN_API
void VulkanGraphicsDriver::SetWindowTitle(const std::string& title) {
    SDL_SetWindowTitle(_sdlWindow, title.c_str());
}

DUSK_VULKAN_API
std::string VulkanGraphicsDriver::GetWindowTitle() {
    return SDL_GetWindowTitle(_sdlWindow);
}

DUSK_VULKAN_API
void VulkanGraphicsDriver::SetWindowSize(const ivec2& size) {
    SDL_SetWindowSize(_sdlWindow, size.x, size.y);
    // glViewport(0, 0, size.x, size.y);
}

DUSK_VULKAN_API
ivec2 VulkanGraphicsDriver::GetWindowSize() {
    ivec2 size;
    SDL_GetWindowSize(_sdlWindow, &size.x, &size.y);
    return size;
}

DUSK_VULKAN_API
void VulkanGraphicsDriver::ProcessEvents() {
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
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
}

} // namespace Dusk
