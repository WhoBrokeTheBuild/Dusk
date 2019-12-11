#include <Dusk/Vulkan/Drivers/GraphicsDriver.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Dusk.hpp>

#include <SDL_vulkan.h>

#undef CreateWindow
#undef near
#undef far

namespace Dusk::Vulkan {

DUSK_VULKAN_API
GraphicsDriver::GraphicsDriver() {
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

    _sdlWindow = SDL_CreateWindow("Dusk", 
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

    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

    DuskLogVerbose("Available Vulkan Extensions:");
    for (const auto& extension : availableExtensions) {
        DuskLogVerbose("\t%s", extension.extensionName);
    }

    static const char * requiredExtensions[64];
    unsigned requiredExtensionCount = sizeof(requiredExtensions) / sizeof(requiredExtensions[0]);

    SDL_bool result = SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &requiredExtensionCount, requiredExtensions);
    if (!result) {
        DuskLogError("SDL_Vulkan_GetInstanceExtensions failed, %s", SDL_GetError());
        return;
    }

    DuskLogVerbose("Required Vulkan Extensions:");
    for (unsigned i = 0; i < requiredExtensionCount; ++i) {
        DuskLogVerbose("\t%s", requiredExtensions[i]);
    }

    VkApplicationInfo appInfo = { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    //appInfo.pApplicationName = "app->GetName()";
    //appInfo.applicationVersion = VK_MAKE_VERSION(
    //    app->GetVersionMajor(),
    //    app->GetVersionMinor(),
    //    app->GetVersionPatch());
    appInfo.pEngineName = "Dusk";
    appInfo.engineVersion = VK_MAKE_VERSION(
        DUSK_VERSION_MAJOR,
        DUSK_VERSION_MINOR,
        DUSK_VERSION_PATCH);
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

    result = SDL_Vulkan_CreateSurface(_sdlWindow, _vkInstance, &_vkWindowSurface);
    if (!result) {
        DuskLogError("SDL_Vulkan_CreateSurface failed, %s", SDL_GetError());
        return;
    }


    // Physical Devices

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        DuskLogError("Failed to find GPUs with Vulkan Support.\n");
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

    // Logical Devices

    VkDeviceQueueCreateInfo queueCreateInfo;


}

DUSK_VULKAN_API
GraphicsDriver::~GraphicsDriver() {
    SDL_DestroyWindow(_sdlWindow);
    SDL_Quit();
}

DUSK_VULKAN_API
void GraphicsDriver::SetWindowTitle(const std::string& title) {
    SDL_SetWindowTitle(_sdlWindow, title.c_str());
}

DUSK_VULKAN_API
std::string GraphicsDriver::GetWindowTitle() {
    return SDL_GetWindowTitle(_sdlWindow);
}

DUSK_VULKAN_API
void GraphicsDriver::SetWindowSize(const ivec2& size) {
    SDL_SetWindowSize(_sdlWindow, size.x, size.y);
    // glViewport(0, 0, size.x, size.y);
}

DUSK_VULKAN_API
ivec2 GraphicsDriver::GetWindowSize() {
    ivec2 size;
    SDL_GetWindowSize(_sdlWindow, &size.x, &size.y);
    return size;
}

DUSK_VULKAN_API
void GraphicsDriver::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0) {
        if (event.type == SDL_QUIT) {
            SetRunning(false);
        }
    }
}


DUSK_VULKAN_API
bool GraphicsDriver::IsDeviceSuitable(const VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
}

DUSK_VULKAN_API
void GraphicsDriver::SwapBuffers() {
    // glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(_sdlWindow);
}

} // namespace Dusk::OpenGL
