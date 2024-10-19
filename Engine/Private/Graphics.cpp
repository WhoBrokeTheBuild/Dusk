#include <Dusk/Graphics.hpp>

#include <Dusk/Array.hpp>
#include <Dusk/Color.hpp>
#include <Dusk/Enumerate.hpp>
#include <Dusk/Exception.hpp>
#include <Dusk/List.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Map.hpp>
#include <Dusk/Set.hpp>
#include <Dusk/ShaderGlobals.hpp>
#include <Dusk/ShaderMaterial.hpp>
#include <Dusk/ShaderTransform.hpp>
#include <Dusk/String.hpp>
#include <Dusk/VulkanBuffer.hpp>
#include <Dusk/VulkanPipeline.hpp>
#include <Dusk/VulkanShader.hpp>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace dusk {
namespace Graphics {

constexpr uint64_t MAX_TIMEOUT = UINT64_MAX;

// Window

SDL_Window * _window = nullptr;

VkExtent2D _windowSize = { 1024, 768 };

// Instance

List<VkLayerProperties> _availableLayerList;

List<VkExtensionProperties> _availableInstanceExtensionList;

DUSK_API
VkInstance Instance = VK_NULL_HANDLE;

VkDebugUtilsMessengerEXT _debugUtilsMessenger = VK_NULL_HANDLE;

// Surface

VkSurfaceKHR _surface = VK_NULL_HANDLE;

// Device

DUSK_API
VkPhysicalDeviceProperties PhysicalDeviceProperties;

DUSK_API
VkPhysicalDeviceFeatures PhysicalDeviceFeatures;

VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;

uint32_t _graphicsQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

uint32_t _presentQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

VkQueue _graphicsQueue;

VkQueue _presentQueue;

DUSK_API
VkDevice Device = VK_NULL_HANDLE;

// Allocator

DUSK_API
VmaAllocator Allocator = VK_NULL_HANDLE;

// Swapchain

VkExtent2D _swapchainExtent;

VkSwapchainKHR _swapchain = VK_NULL_HANDLE;;

VkFormat _swapchainImageFormat = VK_FORMAT_UNDEFINED;

VkColorSpaceKHR _swapchainColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

bool _swapchainPresentModeUndefined = true;

VkPresentModeKHR _swapchainPresentMode;

List<VkImage> _swapchainImageList;

List<VkImageView> _swapchainImageViewList;

List<VkFramebuffer> _framebufferList;

// Depth Buffer

VkFormat _depthImageFormat = VK_FORMAT_UNDEFINED;

VmaAllocation _depthImageAllocation = VK_NULL_HANDLE;

VkImage _depthImage = VK_NULL_HANDLE;

VkImageView _depthImageView = VK_NULL_HANDLE;

// Render Pass

DUSK_API
VkRenderPass RenderPass = VK_NULL_HANDLE;

// Vulkan Command Buffer

VkCommandPool _commandPool;

List<VkCommandBuffer> _commandBufferList;

// Descriptor Pools

// TODO: Make this a list, then pull from .back() when allocating
DUSK_API
VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;

// Descriptor Set Layouts

DUSK_API
VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;

DUSK_API
ShaderGlobals Globals;

VulkanBuffer::Pointer GlobalsBuffer;

// Pipeline Layout

DUSK_API
VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;

// Sync Objects

size_t _currentFrame = 0;

List<VkSemaphore> _imageAvailableSemaphoreList;

List<VkSemaphore> _renderingFinishedSemaphoreList;

List<VkFence> _inFlightFenceList;


static VKAPI_ATTR VkBool32 VKAPI_CALL _VulkanDebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT * callbackData,
    void * userData
)
{
    Log("VkDebugUtilsMessenger", "{} {}",
        callbackData->pMessageIdName,
        callbackData->pMessage
    );

    for (unsigned int i = 0; i < callbackData->objectCount; ++i) {
        Log("VkDebugUtilsMessenger", " - Object #{}: Type: {}, Value: 0x{:016X}, Name: '{}'",
            i,
            VkObjectTypeToString(callbackData->pObjects[i].objectType),
            callbackData->pObjects[i].objectHandle,
            callbackData->pObjects[i].pObjectName
                ? callbackData->pObjects[i].pObjectName
                : "(null)"
        );
    }

    for (unsigned int i = 0; i < callbackData->cmdBufLabelCount; ++i) {
        Log("VkDebugUtilsMessenger", " - Label: #{}, Color: ({}, {}, {}, {}), {}",
            i,
            callbackData->pCmdBufLabels[i].color[0],
            callbackData->pCmdBufLabels[i].color[1],
            callbackData->pCmdBufLabels[i].color[2],
            callbackData->pCmdBufLabels[i].color[3],
            callbackData->pCmdBufLabels[i].pLabelName
                ? callbackData->pCmdBufLabels[i].pLabelName
                : "(null)"
        );
    }

    return VK_FALSE;
}

void initWindow()
{
    if (not SDL_Init(SDL_INIT_VIDEO)) {
        throw Exception("SDL_Init failed, {}", SDL_GetError());
    }

    int sdlVersion = SDL_GetVersion();
    Log(DUSK_ANCHOR, "SDL Version: {}.{}.{}",
        SDL_VERSIONNUM_MAJOR(sdlVersion),
        SDL_VERSIONNUM_MINOR(sdlVersion),
        SDL_VERSIONNUM_MICRO(sdlVersion)
    );

    _window = SDL_CreateWindow(
        "Dusk - HelloWorld",
        _windowSize.width,
        _windowSize.height,
        SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
    );

    // Not needed with SDL_CreateWindow(..., SDL_WINDOW_VULKAN)
    // if (!SDL_Vulkan_LoadLibrary(nullptr)) {
    //     throw Exception("SDL_Vulkan_LoadLibrary failed, {}", SDL_GetError());
    // }
}

void initInstance()
{
    VkResult result;

    List<const char  *> requiredInstanceLayerList = {
        "VK_LAYER_KHRONOS_validation",
    };

    Log(DUSK_ANCHOR, "Required Vulkan Instance Layers:");
    for (const auto& layer : requiredInstanceLayerList) {
        Log(DUSK_ANCHOR, " - {}", layer);
    }

    unsigned int sdlInstanceExtensionCount = 0;
    auto sdlInstanceExtensionNames = SDL_Vulkan_GetInstanceExtensions(&sdlInstanceExtensionCount);

    List<const char *> requiredInstanceExtensionList(
        sdlInstanceExtensionNames,
        sdlInstanceExtensionNames + sdlInstanceExtensionCount
    );

    #if defined(VK_EXT_debug_utils)
        // TODO: Check for extension
        requiredInstanceExtensionList.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    #endif

    Log(DUSK_ANCHOR, "Required Vulkan Instance Extensions:");
    for (const auto& extension : requiredInstanceExtensionList) {
        Log(DUSK_ANCHOR, " - {}", extension);
    }

    auto applicationInfo = VkApplicationInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Hello World",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName = "Dusk",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_1,
    };

    auto instanceCreateInfo = VkInstanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = uint32_t(requiredInstanceLayerList.size()),
        .ppEnabledLayerNames = requiredInstanceLayerList.data(),
        .enabledExtensionCount = uint32_t(requiredInstanceExtensionList.size()),
        .ppEnabledExtensionNames = requiredInstanceExtensionList.data(),
    };

    #if defined(VK_EXT_debug_utils)

        auto debugUtilsMessengerCreateInfo = VkDebugUtilsMessengerCreateInfoEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = (
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                // | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
            ),
            .messageType = (
                  VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
            ),
            .pfnUserCallback = _VulkanDebugMessageCallback,
        };

        // Allows debug messages during instance creation
        instanceCreateInfo.pNext = &debugUtilsMessengerCreateInfo;

    #endif

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &Instance);
    CheckVkResult("vkCreateInstance", result);

    #if defined(VK_EXT_debug_utils)
        auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");
        if (vkCreateDebugUtilsMessengerEXT) {
            vkCreateDebugUtilsMessengerEXT(Instance, &debugUtilsMessengerCreateInfo, nullptr, &_debugUtilsMessenger);
        }
    #endif

    Log(DUSK_ANCHOR, "Vulkan Header Version: {}.{}.{}",
        VK_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE),
        VK_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE),
        VK_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE)
    );
}

void initSurface()
{
    if (!SDL_Vulkan_CreateSurface(_window, Instance, nullptr, &_surface)) {
        throw Exception("SDL_Vulkan_CreateSurface failed, {}", SDL_GetError());
    }
}

void initDevice()
{
    VkResult result;

    Log(DUSK_ANCHOR, "Available Physical Devices:");

    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(Instance, &physicalDeviceCount, nullptr);

    List<VkPhysicalDevice> physicalDeviceList(physicalDeviceCount);
    result = vkEnumeratePhysicalDevices(Instance, &physicalDeviceCount, physicalDeviceList.data());
    CheckVkResult("vkEnumeratePhysicalDevices", result);

    for (const auto& physicalDevice : physicalDeviceList) {
        vkGetPhysicalDeviceProperties(physicalDevice, &PhysicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(physicalDevice, &PhysicalDeviceFeatures);

        bool isSuitable = (
            PhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
            // and _vkPhysicalDeviceFeatures.geometryShader
        );

        // isSuitable = StringView(PhysicalDeviceProperties.deviceName).starts_with("llvmpipe");

        if (isSuitable and not _physicalDevice) {
            _physicalDevice = physicalDevice;
        }

        Log(DUSK_ANCHOR, " - {}", PhysicalDeviceProperties.deviceName);
    }

    if (not _physicalDevice) {
        throw Exception("No suitable physical device found");
    }

    vkGetPhysicalDeviceProperties(_physicalDevice, &PhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(_physicalDevice, &PhysicalDeviceFeatures);

    Log(DUSK_ANCHOR, "Physical Device Name: {}", PhysicalDeviceProperties.deviceName);

    Log(DUSK_ANCHOR, "Physical Vulkan Version: {}.{}.{}",
        VK_VERSION_MAJOR(PhysicalDeviceProperties.apiVersion),
        VK_VERSION_MINOR(PhysicalDeviceProperties.apiVersion),
        VK_VERSION_PATCH(PhysicalDeviceProperties.apiVersion)
    );

    _graphicsQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    _presentQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    Log(DUSK_ANCHOR, "Available Vulkan Queue Families:");

    uint32_t queueFamilyPropertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyPropertyCount, nullptr);

    List<VkQueueFamilyProperties> queueFamilyPropertyList(queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyPropertyCount, queueFamilyPropertyList.data());

    constexpr Tuple<VkQueueFlagBits, StringView> QUEUE_FLAG_NAME_MAP[] = {
        { VK_QUEUE_GRAPHICS_BIT, "GRAPHICS_BIT" },
        { VK_QUEUE_COMPUTE_BIT, "COMPUTE_BIT" },
        { VK_QUEUE_TRANSFER_BIT, "TRANSFER_BIT" },
        { VK_QUEUE_SPARSE_BINDING_BIT, "SPARSE_BINDING_BIT" },
        { VK_QUEUE_PROTECTED_BIT, "PROTECTED_BIT" },
        { VK_QUEUE_VIDEO_DECODE_BIT_KHR, "VIDEO_DECODE_BIT_KHR" },
        { VK_QUEUE_VIDEO_ENCODE_BIT_KHR, "VIDEO_ENCODE_BIT_KHR" },
        { VK_QUEUE_OPTICAL_FLOW_BIT_NV, "OPTICAL_FLOW_BIT_NV" },
    };

    for (auto&& [index, properties] : Enumerate(queueFamilyPropertyList)) {

        VkBool32 hasPresent = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, index, _surface, &hasPresent);

        bool hasGraphics = bool(properties.queueFlags & VK_QUEUE_GRAPHICS_BIT);

        // Pick the first available Queue with Graphics support
        if (hasGraphics and _graphicsQueueFamilyIndex == VK_QUEUE_FAMILY_IGNORED) {
            _graphicsQueueFamilyIndex = index;
        }

        // Pick the first available Queue with Present support
        if (hasPresent and _presentQueueFamilyIndex == VK_QUEUE_FAMILY_IGNORED) {
            _presentQueueFamilyIndex = index;
        }

        // If our Graphics and Present Queues aren't the same, try to find one that supports both
        if (_graphicsQueueFamilyIndex != _presentQueueFamilyIndex) {
            if (hasGraphics and hasPresent) {
                _graphicsQueueFamilyIndex = index;
                _presentQueueFamilyIndex = index;
            }
        }

        List<StringView> queueFlagNameList;
        for (const auto&[ flag, name ] : QUEUE_FLAG_NAME_MAP) {
            if ((properties.queueFlags & flag) == flag) {
                queueFlagNameList.push_back(name);
            }
        }

        Log(DUSK_ANCHOR, " - #{}: {} Queues, Present: {}, Flags: {}",
            index,
            properties.queueCount,
            (hasPresent ? "yes" : "no"),
            StringJoin(queueFlagNameList, " | ")
        );
    }

    Log(DUSK_ANCHOR, "Vulkan Graphics Queue Family Index: #{}", _graphicsQueueFamilyIndex);
    Log(DUSK_ANCHOR, "Vulkan Present Queue Family Index: #{}", _presentQueueFamilyIndex);

    Set<uint32_t> queueFamilyIndexSet = {
        _graphicsQueueFamilyIndex,
        _presentQueueFamilyIndex,
    };

    // Controls the number of queues created and the relative amount of work submitted to each
    constexpr uint32_t QUEUE_COUNT = 1;
    constexpr float QUEUE_PRIORITIES[QUEUE_COUNT] = { 1.0f };

    List<VkDeviceQueueCreateInfo> queueCreateInfoList;
    for (auto index : queueFamilyIndexSet) {
        queueCreateInfoList.push_back(
            VkDeviceQueueCreateInfo{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = index,
                .queueCount = QUEUE_COUNT,
                .pQueuePriorities = QUEUE_PRIORITIES,
            }
        );
    }

    List<const char *> requiredDeviceExtensionList = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    #if defined(VK_EXT_memory_budget)
        requiredDeviceExtensionList.push_back(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);
    #endif

    #if defined(VK_EXT_extended_dynamic_state)
        requiredDeviceExtensionList.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME);
    #endif

    auto deviceCreateInfo = VkDeviceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = uint32_t(queueCreateInfoList.size()),
        .pQueueCreateInfos = queueCreateInfoList.data(),
        .enabledExtensionCount = uint32_t(requiredDeviceExtensionList.size()),
        .ppEnabledExtensionNames = requiredDeviceExtensionList.data(),
    };

    result = vkCreateDevice(_physicalDevice, &deviceCreateInfo, nullptr, &Device);
    CheckVkResult("vkCreateDevice", result);

    vkGetDeviceQueue(Device, _graphicsQueueFamilyIndex, 0, &_graphicsQueue);
    vkGetDeviceQueue(Device, _presentQueueFamilyIndex, 0, &_presentQueue);
}

void initAllocator()
{
    VkResult result;

    VmaAllocatorCreateFlags allocatorCreateFlags = 0;

    #if defined(VK_EXT_memory_budget)
        // TODO: Check for extension
        allocatorCreateFlags |= VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    #endif

    auto allocatorCreateInfo = VmaAllocatorCreateInfo{
        .flags = allocatorCreateFlags,
        .physicalDevice = _physicalDevice,
        .device = Device,
        .instance = Instance,
        .vulkanApiVersion = VK_API_VERSION_1_3,
    };

    result = vmaCreateAllocator(&allocatorCreateInfo, &Allocator);
    CheckVkResult("vmaCreateAllocator", result);

    constexpr Tuple<VkMemoryPropertyFlagBits, StringView> MEMORY_PROPERTY_FLAG_NAME_MAP[] = {
        { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, "DEVICE_LOCAL_BIT" },
        { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, "HOST_VISIBLE_BIT" },
        { VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, "HOST_COHERENT_BIT" },
        { VK_MEMORY_PROPERTY_HOST_CACHED_BIT, "HOST_CACHED_BIT" },
        { VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT, "LAZILY_ALLOCATED_BIT" },
        { VK_MEMORY_PROPERTY_PROTECTED_BIT, "PROTECTED_BIT" },
        { VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD, "DEVICE_COHERENT_BIT_AMD" },
        { VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD, "DEVICE_UNCACHED_BIT_AMD" },
        { VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV, "RDMA_CAPABLE_BIT_NV" },
    };

    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memoryProperties);

    List<VmaBudget> heapBudgetList(memoryProperties.memoryHeapCount);
    vmaGetHeapBudgets(Allocator, heapBudgetList.data());

    for (auto&& [heapIndex, heap] : Enumerate(heapBudgetList)) {
        Log(DUSK_ANCHOR, "Vulkan Memory Heap #{}: {}", heapIndex, FormatBytesHumanReadable(heap.budget));

        for (auto&& [typeIndex, type] : Enumerate(memoryProperties.memoryTypes)) {
            if (type.heapIndex == heapIndex and type.propertyFlags) {

                List<StringView> propertyFlagNameList;
                for (const auto&[ flag, name ] : MEMORY_PROPERTY_FLAG_NAME_MAP) {
                    if ((type.propertyFlags & flag) == flag) {
                        propertyFlagNameList.push_back(name);
                    }
                }

                Log(DUSK_ANCHOR, " - Type #{}: {}",
                    typeIndex,
                    StringJoin(propertyFlagNameList, " | ")
                );
            }
        }
    }
}

void initDepthBuffer()
{
    VkResult result;

    constexpr Tuple<VkFormat, StringView> DEPTH_FORMAT_NAME_MAP[] = {
        { VK_FORMAT_D32_SFLOAT, "D32_SFLOAT" },
        { VK_FORMAT_D32_SFLOAT_S8_UINT, "D32_SFLOAT_S8_UINT" },
        { VK_FORMAT_D24_UNORM_S8_UINT, "D24_UNORM_S8_UINT" },
        { VK_FORMAT_D16_UNORM, "D16_UNORM" },
        { VK_FORMAT_D16_UNORM_S8_UINT, "D16_UNORM_S8_UINT" },
    };

    if (_depthImageFormat == VK_FORMAT_UNDEFINED) {
        for (const auto& [ format, name ] : DEPTH_FORMAT_NAME_MAP) {
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &properties);

            bool hasDepthStencil = bool(properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

            // TODO: Stencil?

            if (hasDepthStencil) {
                Log(DUSK_ANCHOR, "Vulkan Depth Buffer Image Format: {}", name);

                _depthImageFormat = format;
                break;
            }
        }

        if (_depthImageFormat == VK_FORMAT_UNDEFINED) {
            throw Exception("Unable to find suitable depth buffer image format");
        }
    }

    vkDestroyImageView(Device, _depthImageView, nullptr);
    vmaDestroyImage(Allocator, _depthImage, _depthImageAllocation);

    _depthImageView = VK_NULL_HANDLE;
    _depthImage = VK_NULL_HANDLE;
    _depthImageAllocation = VK_NULL_HANDLE;

    auto imageCreateInfo = VkImageCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = _depthImageFormat,
        .extent = VkExtent3D{
            .width = _swapchainExtent.width,
            .height = _swapchainExtent.height,
            .depth = 1
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    auto allocationCreateInfo = VmaAllocationCreateInfo{
        .usage = VMA_MEMORY_USAGE_GPU_ONLY,
        .requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    };

    result = vmaCreateImage(
        Allocator,
        &imageCreateInfo,
        &allocationCreateInfo,
        &_depthImage,
        &_depthImageAllocation,
        nullptr
    );
    CheckVkResult("vmaCreateImage", result);

    auto imageViewCreateInfo = VkImageViewCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = _depthImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = _depthImageFormat,
        .subresourceRange = VkImageSubresourceRange{
            .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    result = vkCreateImageView(Device, &imageViewCreateInfo, nullptr, &_depthImageView);
    CheckVkResult("vkCreateImageView", result);
}

void initRenderPass()
{
    VkResult result;

    auto colorAttachmentDescription = VkAttachmentDescription{
        .format = _swapchainImageFormat,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    auto depthAttachmentDescription = VkAttachmentDescription{
        .format = _depthImageFormat,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    auto colorAttachmentReference = VkAttachmentReference{
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    auto depthAttachmentReference = VkAttachmentReference{
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    auto subpassDescription = VkSubpassDescription{
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentReference,
        .pDepthStencilAttachment = &depthAttachmentReference,
    };

    Array<VkSubpassDependency, 1> subpassDependencyList = {
        VkSubpassDependency{
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = (
                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT
            ),
            .dstStageMask = (
                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
            ),
            .srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            .dstAccessMask = (
                  VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
                | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
            ),  
        },
        // VkSubpassDependency()
        //     .setSrcSubpass(VK_SUBPASS_EXTERNAL)
        //     .setDstSubpass(0)
        //     .setSrcStageMask(VkPipelineStageFlagBits::eColorAttachmentOutput)
        //     .setDstStageMask(VkPipelineStageFlagBits::eColorAttachmentOutput)
        //     .setSrcAccessMask({})
        //     .setDstAccessMask(VkAccessFlagBits::eColorAttachmentWrite),
        // VkSubpassDependency()
        //     .setSrcSubpass(VK_SUBPASS_EXTERNAL)
        //     .setDstSubpass(0)
        //     .setSrcStageMask(
        //         VkPipelineStageFlagBits::eEarlyFragmentTests |
        //         VkPipelineStageFlagBits::eLateFragmentTests
        //     )
        //     .setDstStageMask(
        //         VkPipelineStageFlagBits::eEarlyFragmentTests |
        //         VkPipelineStageFlagBits::eLateFragmentTests
        //     )
        //     .setSrcAccessMask({})
        //     .setDstAccessMask(VkAccessFlagBits::eDepthStencilAttachmentWrite),
    };

    Array<VkAttachmentDescription, 2> attachmentList = {
        colorAttachmentDescription,
        depthAttachmentDescription,
    };

    vkDestroyRenderPass(Device, RenderPass, nullptr);

    auto renderPassCreateInfo = VkRenderPassCreateInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = uint32_t(attachmentList.size()),
        .pAttachments = attachmentList.data(),
        .subpassCount = 1,
        .pSubpasses = &subpassDescription,
        .dependencyCount = uint32_t(subpassDependencyList.size()),
        .pDependencies = subpassDependencyList.data(),
    };

    result = vkCreateRenderPass(Device, &renderPassCreateInfo, nullptr, &RenderPass);
    CheckVkResult("vkCreateRenderPass", result);
}

void initDescriptorPool()
{
    VkResult result;

    ///
    /// Descriptor Pool
    ///

    auto descriptorPoolSize = VkDescriptorPoolSize{
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1000, // TODO: Improve
    };

    auto descriptorPoolCreateInfo = VkDescriptorPoolCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = 1000,
        .poolSizeCount = 1,
        .pPoolSizes = &descriptorPoolSize,
    };

    result = vkCreateDescriptorPool(Device, &descriptorPoolCreateInfo, nullptr, &DescriptorPool);
    CheckVkResult("vkCreateDescriptorPool", result);
}

void initDescriptorSetLayout()
{
    VkResult result;

    Array<VkDescriptorSetLayoutBinding, 2> _descriptorSetLayoutBindingList = {
        VkDescriptorSetLayoutBinding{
            .binding = ShaderGlobals::Binding,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        },
        VkDescriptorSetLayoutBinding{
            .binding = ShaderTransform::Binding,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        },
    };

    // VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    // VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER

    auto _descriptorSetLayoutCreateInfo = VkDescriptorSetLayoutCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = uint32_t(_descriptorSetLayoutBindingList.size()),
        .pBindings = _descriptorSetLayoutBindingList.data(),
    };

    result = vkCreateDescriptorSetLayout(
        Device,
        &_descriptorSetLayoutCreateInfo,
        nullptr,
        &DescriptorSetLayout
    );
    CheckVkResult("vkCreateDescriptorSetLayout", result);

    GlobalsBuffer.reset(new VulkanBuffer());
    GlobalsBuffer->Create(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        sizeof(ShaderGlobals)
    );
}

void initPipelineLayout()
{
    VkResult result;

    auto pipelineLayoutCreateInfo = VkPipelineLayoutCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &DescriptorSetLayout,
    };

    result = vkCreatePipelineLayout(Device, &pipelineLayoutCreateInfo, nullptr, &PipelineLayout);
    CheckVkResult("vkCreatePipelineLayout", result);
}

void initFramebuffers()
{
    VkResult result;

    for (auto& framebuffer : _framebufferList) {
        vkDestroyFramebuffer(Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }

    _framebufferList.resize(_swapchainImageViewList.size());

    for (auto&& [index, imageView] : Enumerate(_swapchainImageViewList)) {
        Array<VkImageView, 2> attachmentList = {
            imageView,
            _depthImageView,
        };

        auto framebufferCreateInfo = VkFramebufferCreateInfo{
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = RenderPass,
            .attachmentCount = uint32_t(attachmentList.size()),
            .pAttachments = attachmentList.data(),
            .width = _swapchainExtent.width,
            .height = _swapchainExtent.height,
            .layers = 1,
        };

        result = vkCreateFramebuffer(
            Device,
            &framebufferCreateInfo,
            nullptr,
            &_framebufferList[index]
        );
        CheckVkResult("vkCreateFramebuffer", result);
    }
}

void initCommandBuffers()
{
    VkResult result;

    if (not _commandBufferList.empty()) {
        vkFreeCommandBuffers(
            Device,
            _commandPool,
            _commandBufferList.size(), _commandBufferList.data()
        );
    }

    vkDestroyCommandPool(Device, _commandPool, nullptr);
    _commandPool = VK_NULL_HANDLE;

    auto commandPoolCreateInfo = VkCommandPoolCreateInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = _graphicsQueueFamilyIndex,
    };

    result = vkCreateCommandPool(Device, &commandPoolCreateInfo, nullptr, &_commandPool);
    CheckVkResult("vkCreateCommandPool", result);

    _commandBufferList.resize(_swapchainImageList.size());

    auto commandBufferAllocateInfo = VkCommandBufferAllocateInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = _commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = uint32_t(_commandBufferList.size()),
    };

    result = vkAllocateCommandBuffers(Device, &commandBufferAllocateInfo, _commandBufferList.data());
    CheckVkResult("vkAllocateCommandBuffers", result);
}

// TODO: Remove
std::function<void(VkCommandBuffer)> _renderCallback;

void fillCommandBuffers()
{
    VkResult result;

    for (auto&& [index, commandBuffer] : Enumerate(_commandBufferList)) {
        auto commandBufferBeginInfo = VkCommandBufferBeginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        };

        result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
        CheckVkResult("vkBeginCommandBuffer", result);

        vec4 clearColor = Color::CornflowerBlue;

        // If our surface is sRGB, Vulkan will try to convert our color to sRGB
        // This fails and washes out the color, so we convert to linear to account for it  
        if (_swapchainColorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            clearColor = Color::ToLinear(clearColor);
        }

        Array<VkClearValue, 2> clearValueList = {
            VkClearValue{
                .color = VkClearColorValue{
                    .float32 = {
                        clearColor[0],
                        clearColor[1],
                        clearColor[2],
                        clearColor[3]
                    },
                },
            },
            VkClearValue{
                .depthStencil = VkClearDepthStencilValue{
                    .depth = 1.0f,
                    .stencil = 0,
                },
            }
        };

        auto renderArea = VkRect2D{
            .offset = { 0, 0 },
            .extent = _swapchainExtent,
        };

        auto renderPassBeginInfo = VkRenderPassBeginInfo{
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = RenderPass,
            .framebuffer = _framebufferList[index],
            .renderArea = renderArea,
            .clearValueCount = uint32_t(clearValueList.size()),
            .pClearValues = clearValueList.data(),
        };

        vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        auto viewport = VkViewport{
            .x = 0,
            .y = float(_swapchainExtent.height),
            .width = float(_swapchainExtent.width),
            .height = -float(_swapchainExtent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        auto scissor = VkRect2D{
            .offset = { 0, 0 },
            .extent = _swapchainExtent,
        };

        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        if (_renderCallback) {
            _renderCallback(commandBuffer);
        }

        vkCmdEndRenderPass(commandBuffer);

        result = vkEndCommandBuffer(commandBuffer);
        CheckVkResult("vkEndCommandBuffer", result);
    }
}

void initSyncObjects()
{
    VkResult result;

    for (auto& fence : _inFlightFenceList) {
        vkDestroyFence(Device, fence, nullptr);
        fence = VK_NULL_HANDLE;
    }

    for (auto& semaphore : _imageAvailableSemaphoreList) {
        vkDestroySemaphore(Device, semaphore, nullptr);
        semaphore = VK_NULL_HANDLE;
    }

    for (auto& semaphore : _renderingFinishedSemaphoreList) {
        vkDestroySemaphore(Device, semaphore, nullptr);
        semaphore = VK_NULL_HANDLE;
    }

    size_t backbufferCount = _swapchainImageViewList.size();

    _inFlightFenceList.resize(backbufferCount);
    _imageAvailableSemaphoreList.resize(backbufferCount);
    _renderingFinishedSemaphoreList.resize(backbufferCount);

    auto semaphoreCreateInfo = VkSemaphoreCreateInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };

    auto fenceCreateInfo = VkFenceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    for (size_t i = 0; i < backbufferCount; ++i) {
        result = vkCreateFence(Device, &fenceCreateInfo, nullptr, &_inFlightFenceList[i]);
        CheckVkResult("vkCreateFence", result);

        result = vkCreateSemaphore(Device, &semaphoreCreateInfo, nullptr, &_imageAvailableSemaphoreList[i]);
        CheckVkResult("vkCreateSemaphore", result);

        result = vkCreateSemaphore(Device, &semaphoreCreateInfo, nullptr, &_renderingFinishedSemaphoreList[i]);
        CheckVkResult("vkCreateSemaphore", result);
    }
}

void initSwapchain()
{
    VkResult result;

    result = vkDeviceWaitIdle(Device);
    CheckVkResult("vkDeviceWaitIdle", result);

    // Image Format

    if (_swapchainImageFormat == VK_FORMAT_UNDEFINED) {

        uint32_t surfaceFormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &surfaceFormatCount, nullptr);

        List<VkSurfaceFormatKHR> surfaceFormatList(surfaceFormatCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &surfaceFormatCount, surfaceFormatList.data());
        CheckVkResult("vkGetPhysicalDeviceSurfaceFormatsKHR", result);

        if (surfaceFormatList.empty()) {
            throw Exception("No surface formats available");
        }

        Log(DUSK_ANCHOR, "Available Vulkan Surface Formats:");
        for (const auto& surfaceFormat : surfaceFormatList) {
            bool isPreferredFormat = (
                   surfaceFormat.format == VK_FORMAT_R8G8B8A8_SRGB
                or surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB
            );

            bool isSRGB = (surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);

            if (isPreferredFormat and isSRGB) {
                _swapchainImageFormat = surfaceFormat.format;
                _swapchainColorSpace = surfaceFormat.colorSpace;
            }

            Log(DUSK_ANCHOR, " - {} {}",
                VkFormatToString(surfaceFormat.format),
                VkColorSpaceToString(surfaceFormat.colorSpace)
            );
        }

        if (_swapchainImageFormat == VK_FORMAT_UNDEFINED) {
            Log(DUSK_ANCHOR, "Unable to find a preferred surface format");

            const auto& surfaceFormat = surfaceFormatList.front();
            _swapchainImageFormat = surfaceFormat.format;
            _swapchainColorSpace = surfaceFormat.colorSpace;
        }

        Log(DUSK_ANCHOR, "Vulkan Swapchain Image Format: {} {}",
            VkFormatToString(_swapchainImageFormat),
            VkColorSpaceToString(_swapchainColorSpace)
        );
    }

    // Image Extent

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, _surface, &surfaceCapabilities);
    CheckVkResult("vkGetPhysicalDeviceSurfaceCapabilitiesKHR", result);

    _swapchainExtent = surfaceCapabilities.currentExtent;
    if (_swapchainExtent.width == UINT32_MAX) {
        _swapchainExtent.width = std::clamp(
            _windowSize.width,
            surfaceCapabilities.minImageExtent.width,
            surfaceCapabilities.maxImageExtent.width
        );

        _swapchainExtent.height = std::clamp(
            _windowSize.height,
            surfaceCapabilities.minImageExtent.height,
            surfaceCapabilities.maxImageExtent.height
        );
    }

    Log(DUSK_ANCHOR, "Vulkan Swapchain Extent: {}x{}",
        _swapchainExtent.width,
        _swapchainExtent.height
    );

    // Present Mode

    if (_swapchainPresentModeUndefined) {
        // FIFO is the only present mode required to be supported
        // New frames are added to a FIFO queue, and taken off when presented
        _swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        
        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, _surface, &presentModeCount, nullptr);

        List<VkPresentModeKHR> presentModeList(presentModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(
            _physicalDevice,
            _surface,
            &presentModeCount,
            presentModeList.data()
        );
        CheckVkResult("vkGetPhysicalDeviceSurfacePresentModesKHR", result);
        
        Log(DUSK_ANCHOR, "Available Vulkan Present Modes:");
        for (const auto& mode : presentModeList) {
            Log(DUSK_ANCHOR, " - {}", VkPresentModeToString(mode));
        }

        // Mailbox is like FIFO, but automatically discards extra images
        if (ListContains(presentModeList, VK_PRESENT_MODE_MAILBOX_KHR)) {
            _swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        }

        _swapchainPresentModeUndefined = false;
    }

    Log(DUSK_ANCHOR, "Vulkan Swapchain Present Mode: {}", VkPresentModeToString(_swapchainPresentMode));

    // Actual

    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 and imageCount > surfaceCapabilities.maxImageCount) {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    auto oldSwapchain = _swapchain;

    auto swapchainCreateInfo = VkSwapchainCreateInfoKHR{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = _surface,
        .minImageCount = imageCount,
        .imageFormat = _swapchainImageFormat,
        .imageColorSpace = _swapchainColorSpace,
        .imageExtent = _swapchainExtent,
        .imageArrayLayers = 1, // Always 1 for 3D applications
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = surfaceCapabilities.currentTransform,
        .presentMode = _swapchainPresentMode,
        .clipped = true,
        .oldSwapchain = oldSwapchain,
    };

    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    // if (surfaceCapabilities.supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::ePreMultiplied) {
    //     swapchainCreateInfo.setCompositeAlpha(VkCompositeAlphaFlagBitsKHR::ePreMultiplied);
    // }
    // else if (surfaceCapabilities.supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::ePostMultiplied) {
    //     swapchainCreateInfo.setCompositeAlpha(VkCompositeAlphaFlagBitsKHR::ePostMultiplied);
    // }
    // else if (surfaceCapabilities.supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::eInherit) {
    //     swapchainCreateInfo.setCompositeAlpha(VkCompositeAlphaFlagBitsKHR::eInherit);
    // }

    uint32_t queueFamilyIndexList[] = {
        _graphicsQueueFamilyIndex,
        _presentQueueFamilyIndex,
    };

    if (_graphicsQueueFamilyIndex != _presentQueueFamilyIndex) {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndexList;
    }
    else {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    Log(DUSK_ANCHOR, "Vulkan Swapchain Image Count: {}", swapchainCreateInfo.minImageCount);

    result = vkCreateSwapchainKHR(Device, &swapchainCreateInfo, nullptr, &_swapchain);
    CheckVkResult("vkCreateSwapchainKHR", result);

    vkDestroySwapchainKHR(Device, oldSwapchain, nullptr);

    // Image List

    uint32_t swapchainImageCount = 0;
    vkGetSwapchainImagesKHR(Device, _swapchain, &swapchainImageCount, nullptr);

    _swapchainImageList.resize(swapchainImageCount);
    result = vkGetSwapchainImagesKHR(Device, _swapchain, &swapchainImageCount, _swapchainImageList.data());
    CheckVkResult("vkGetSwapchainImagesKHR", result);

    for (auto& imageView : _swapchainImageViewList) {
        vkDestroyImageView(Device, imageView, nullptr);
    }

    _swapchainImageViewList.resize(_swapchainImageList.size(), VK_NULL_HANDLE);

    auto imageViewCreateInfo = VkImageViewCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = _swapchainImageFormat,
        .subresourceRange = VkImageSubresourceRange{
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    for (const auto&& [index, image] : Enumerate(_swapchainImageList)) {
        imageViewCreateInfo.image = image;
        result = vkCreateImageView(Device, &imageViewCreateInfo, nullptr, &_swapchainImageViewList[index]);
        CheckVkResult("vkCreateImageView", result);
    }

    initDepthBuffer();
    initRenderPass();
    initFramebuffers();
    initCommandBuffers();
    fillCommandBuffers(); // TODO: Improve
    initSyncObjects();
}

DUSK_API
void SetRenderCallback(std::function<void(VkCommandBuffer)> renderCallback)
{
    _renderCallback = renderCallback;
    fillCommandBuffers();
}

DUSK_API
void Init()
{
    initWindow();
    initInstance();
    initSurface();
    initDevice();
    initAllocator();
    initDescriptorPool();
    initDescriptorSetLayout();
    initPipelineLayout();

    initSwapchain();
    initSwapchain(); // Test swap chain recreation
}

DUSK_API
void Term()
{
    VkResult result;

    result = vkDeviceWaitIdle(Device);
    CheckVkResult("vkDeviceWaitIdle", result);

    // Sync Objects

    for (auto fence : _inFlightFenceList) {
        vkDestroyFence(Device, fence, nullptr);
    }

    _inFlightFenceList.clear();

    for (auto semaphore : _renderingFinishedSemaphoreList) {
        vkDestroySemaphore(Device, semaphore, nullptr);
    }

    _renderingFinishedSemaphoreList.clear();

    for (auto semaphore : _imageAvailableSemaphoreList) {
        vkDestroySemaphore(Device, semaphore, nullptr);
    }

    _imageAvailableSemaphoreList.clear();

    // Framebuffers

    for (auto& framebuffer : _framebufferList) {
        vkDestroyFramebuffer(Device, framebuffer, nullptr);
    }

    _framebufferList.clear();

    // Command Buffers

    vkFreeCommandBuffers(Device, _commandPool, _commandBufferList.size(), _commandBufferList.data());
    _commandBufferList.clear();

    vkDestroyCommandPool(Device, _commandPool, nullptr);
    _commandPool = VK_NULL_HANDLE;

    // Render Pass

    vkDestroyRenderPass(Device, RenderPass, nullptr);
    RenderPass = VK_NULL_HANDLE;

    // Depth Buffer

    vkDestroyImageView(Device, _depthImageView, nullptr);
    _depthImageView = VK_NULL_HANDLE;

    vmaDestroyImage(Allocator, _depthImage, _depthImageAllocation);
    _depthImage = VK_NULL_HANDLE;
    _depthImageAllocation = VK_NULL_HANDLE;

    // Swapchain

    for (auto& imageView : _swapchainImageViewList) {
        vkDestroyImageView(Device, imageView, nullptr);
    }

    _swapchainImageViewList.clear();

    vkDestroySwapchainKHR(Device, _swapchain, nullptr);
    _swapchain = VK_NULL_HANDLE;

    // Pipeline Layout

    vkDestroyPipelineLayout(Device, PipelineLayout, nullptr);
    PipelineLayout = VK_NULL_HANDLE;

    // Descriptor Set Layouts

    GlobalsBuffer->Destroy();

    vkDestroyDescriptorSetLayout(Device, DescriptorSetLayout, nullptr);
    DescriptorSetLayout = VK_NULL_HANDLE;

    // Descriptor Pools

    vkDestroyDescriptorPool(Device, DescriptorPool, nullptr);
    DescriptorPool = VK_NULL_HANDLE;

    // Allocator

    vmaDestroyAllocator(Allocator);
    Allocator = VK_NULL_HANDLE;

    // Device

    vkDestroyDevice(Device, nullptr);
    Device = VK_NULL_HANDLE;

    // Surface

    vkDestroySurfaceKHR(Instance, _surface, nullptr);
    _surface = VK_NULL_HANDLE;

    // Instance

    auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");
    if (vkDestroyDebugUtilsMessengerEXT) {
        vkDestroyDebugUtilsMessengerEXT(Instance, _debugUtilsMessenger, nullptr);
    }

    vkDestroyInstance(Instance, nullptr);
    Instance = VK_NULL_HANDLE;

    // Window

    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    SDL_Quit();
}

DUSK_API
void Render()
{
    VkResult result;

    static bool updateSwapchain = false;

    if (updateSwapchain) {
        Log(DUSK_ANCHOR, "Regenerating Swapchain");
        initSwapchain();
        updateSwapchain = false;
    }

    vmaSetCurrentFrameIndex(Allocator, _currentFrame);

    result = vkWaitForFences(
        Device,
        1, &_inFlightFenceList[_currentFrame], 
        true,
        MAX_TIMEOUT
    );
    CheckVkResult("vkWaitForFences", result);

    result = vkResetFences(Device, 1, &_inFlightFenceList[_currentFrame]);
    CheckVkResult("vkResetFences", result);

    Globals.Resolution.x = _swapchainExtent.width;
    Globals.Resolution.y = _swapchainExtent.height;
    SDL_GetMouseState(&Globals.Mouse.x, &Globals.Mouse.y);
    Globals.FrameCount = _currentFrame;
    // Globals.DeltaTime
    // Globals.TotalTime
    // Globals.FrameSpeedRatio

    // TODO: Move? Wrap with Fences?
    GlobalsBuffer->WriteTo(0, sizeof(Globals), &Globals);

    uint32_t imageIndex;

    result = vkAcquireNextImageKHR(
        Device,
        _swapchain,
        MAX_TIMEOUT,
        _imageAvailableSemaphoreList[_currentFrame],
        nullptr,
        &imageIndex
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        updateSwapchain = true;
    }
    else {
        CheckVkResult("vkAcquireNextImageKHR", result);
    }

    VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    auto submitInfo = VkSubmitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = (VkSemaphore *)&_imageAvailableSemaphoreList[_currentFrame],
        .pWaitDstStageMask = &waitDstStageMask,
        .commandBufferCount = 1,
        .pCommandBuffers = (VkCommandBuffer *)&_commandBufferList[imageIndex],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = (VkSemaphore *)&_renderingFinishedSemaphoreList[_currentFrame],
    };

    result = vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFenceList[_currentFrame]);
    CheckVkResult("vkQueueSubmit", result);

    auto presentInfo = VkPresentInfoKHR{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = (VkSemaphore *)&_renderingFinishedSemaphoreList[_currentFrame],
        .swapchainCount = 1,
        .pSwapchains = (VkSwapchainKHR *)&_swapchain,
        .pImageIndices = &imageIndex,
    };

    result = vkQueuePresentKHR(_presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        updateSwapchain = true;
    }
    else {
        CheckVkResult("vkQueuePresentKHR", result);
    }

    _currentFrame = (_currentFrame + 1) % _inFlightFenceList.size();

    // TODO: Fix
    result = vkDeviceWaitIdle(Device);
    CheckVkResult("vkDeviceWaitIdle", result);
    fillCommandBuffers();
}

DUSK_API
void HandleEvent(SDL_Event * event)
{
    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        _windowSize.width = event->window.data1;
        _windowSize.height = event->window.data2;

        initSwapchain();
    }
}

DUSK_API
VkExtent2D GetWindowSize()
{
    return _windowSize;
}

DUSK_API
void CopyBuffer(VkBuffer source, VkBuffer destination, VkBufferCopy region)
{
    VkResult result;

    auto commandBufferAllocateInfo = VkCommandBufferAllocateInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = _commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    result = vkAllocateCommandBuffers(Device, &commandBufferAllocateInfo, &commandBuffer);
    CheckVkResult("vkAllocateCommandBuffers", result);

    auto commandBufferBeginInfo = VkCommandBufferBeginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    CheckVkResult("vkBeginCommandBuffer", result);

    vkCmdCopyBuffer(commandBuffer, source, destination, 1, &region);

    result = vkEndCommandBuffer(commandBuffer);
    CheckVkResult("vkEndCommandBuffer", result);

    auto submitInfo = VkSubmitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer,
    };

    result = vkQueueSubmit(_graphicsQueue, 1, &submitInfo, nullptr);
    CheckVkResult("vkQueueSubmit", result);

    result = vkQueueWaitIdle(_graphicsQueue);
    CheckVkResult("vkQueueWaitIdle", result);

    vkFreeCommandBuffers(Device, _commandPool, 1, &commandBuffer);
}

DUSK_API
void CopyBufferToImage(VkBuffer source, VkImage destination, VkBufferImageCopy region)
{
    VkResult result;

    auto commandBufferAllocateInfo = VkCommandBufferAllocateInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = _commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    result = vkAllocateCommandBuffers(Device, &commandBufferAllocateInfo, &commandBuffer);
    CheckVkResult("vkAllocateCommandBuffers", result);

    auto commandBufferBeginInfo = VkCommandBufferBeginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    auto subresourceRange = VkImageSubresourceRange{
        .aspectMask = region.imageSubresource.aspectMask,
        .baseMipLevel = 0,
        .levelCount = region.imageSubresource.mipLevel + 1,
        .baseArrayLayer = region.imageSubresource.baseArrayLayer,
        .layerCount = region.imageSubresource.layerCount,
    };

    auto imageMemoryBarrier = VkImageMemoryBarrier{
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = destination,
        .subresourceRange = subresourceRange,
    };

    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    CheckVkResult("vkBeginCommandBuffer", result);

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VkDependencyFlags(0),
        0, nullptr,
        0, nullptr,
        1, &imageMemoryBarrier
    );

    vkCmdCopyBufferToImage(commandBuffer,
        source,
        destination,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1, &region
    );

    result = vkEndCommandBuffer(commandBuffer);
    CheckVkResult("vkEndCommandBuffer", result);

    auto submitInfo = VkSubmitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer,
    };

    result = vkQueueSubmit(_graphicsQueue, 1, &submitInfo, nullptr);
    CheckVkResult("vkQueueSubmit", result);

    result = vkQueueWaitIdle(_graphicsQueue);
    CheckVkResult("vkQueueWaitIdle", result);

    vkFreeCommandBuffers(Device, _commandPool, 1, &commandBuffer);
}

} // namespace Graphics
} // namespace dusk