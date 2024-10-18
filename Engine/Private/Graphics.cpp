#include <Dusk/Graphics.hpp>

#include <Dusk/Array.hpp>
#include <Dusk/Color.hpp>
#include <Dusk/Enumerate.hpp>
#include <Dusk/Exception.hpp>
#include <Dusk/List.hpp>
#include <Dusk/Set.hpp>
#include <Dusk/ShaderGlobals.hpp>
#include <Dusk/ShaderMaterial.hpp>
#include <Dusk/ShaderTransform.hpp>
#include <Dusk/String.hpp>
#include <Dusk/VulkanBuffer.hpp>
#include <Dusk/VulkanPipeline.hpp>
#include <Dusk/VulkanShader.hpp>
#include <Dusk/Log.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace dusk {
namespace Graphics {

constexpr uint64_t MAX_TIMEOUT = UINT64_MAX;

// Window

SDL_Window * _window = nullptr;

vk::Extent2D _windowSize = { 1024, 768 };

// Instance

List<vk::LayerProperties> _availableLayerList;

List<vk::ExtensionProperties> _availableInstanceExtensionList;

DUSK_API
vk::Instance Instance = VK_NULL_HANDLE;

vk::DebugUtilsMessengerEXT _debugUtilsMessenger = VK_NULL_HANDLE;

// Surface

vk::SurfaceKHR _surface = VK_NULL_HANDLE;

// Device

DUSK_API
vk::PhysicalDeviceProperties PhysicalDeviceProperties;

DUSK_API
vk::PhysicalDeviceFeatures PhysicalDeviceFeatures;

vk::PhysicalDevice _physicalDevice = VK_NULL_HANDLE;

uint32_t _graphicsQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

uint32_t _presentQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

vk::Queue _graphicsQueue;

vk::Queue _presentQueue;

DUSK_API
vk::Device Device = VK_NULL_HANDLE;

// Allocator

DUSK_API
VmaAllocator Allocator = VK_NULL_HANDLE;

// Swapchain

vk::Extent2D _swapchainExtent;

vk::SwapchainKHR _swapchain = VK_NULL_HANDLE;;

vk::Format _swapchainImageFormat = vk::Format::eUndefined;

vk::ColorSpaceKHR _swapchainColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;

bool _swapchainPresentModeUndefined = true;

vk::PresentModeKHR _swapchainPresentMode;

List<vk::Image> _swapchainImageList;

List<vk::ImageView> _swapchainImageViewList;

List<vk::Framebuffer> _framebufferList;

// Depth Buffer

vk::Format _depthImageFormat = vk::Format::eUndefined;

VmaAllocation _depthImageAllocation = VK_NULL_HANDLE;

vk::Image _depthImage = VK_NULL_HANDLE;

vk::ImageView _depthImageView = VK_NULL_HANDLE;

// Render Pass

DUSK_API
vk::RenderPass RenderPass = VK_NULL_HANDLE;

// Vulkan Command Buffer

vk::CommandPool _commandPool;

List<vk::CommandBuffer> _commandBufferList;

// Descriptor Pools

// TODO: Make this a list, then pull from .back() when allocating
DUSK_API
vk::DescriptorPool DescriptorPool = VK_NULL_HANDLE;

// Descriptor Set Layouts

DUSK_API
vk::DescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;

DUSK_API
ShaderGlobals Globals;

VulkanBuffer::Pointer GlobalsBuffer;

// Pipeline Layout

DUSK_API
vk::PipelineLayout PipelineLayout = VK_NULL_HANDLE;

// Sync Objects

size_t _currentFrame = 0;

List<vk::Semaphore> _imageAvailableSemaphoreList;

List<vk::Semaphore> _renderingFinishedSemaphoreList;

List<vk::Fence> _inFlightFenceList;



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
            vk::to_string(static_cast<vk::ObjectType>(callbackData->pObjects[i].objectType)),
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
    
    VULKAN_HPP_DEFAULT_DISPATCHER.init(
        (PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr()
    );
}

void initInstance()
{
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
    
    auto applicationInfo = vk::ApplicationInfo()
        .setPApplicationName("Hello World")
        .setApplicationVersion(VK_MAKE_VERSION(0, 0, 1))
        .setPEngineName("Dusk")
        .setEngineVersion(VK_MAKE_VERSION(0, 0, 1))
        .setApiVersion(VK_API_VERSION_1_1);

    auto instanceCreateInfo = vk::InstanceCreateInfo()
        .setPApplicationInfo(&applicationInfo)
        .setPEnabledLayerNames(requiredInstanceLayerList)
        .setPEnabledExtensionNames(requiredInstanceExtensionList);

    #if defined(VK_EXT_debug_utils)

        auto debugUtilsMessengerCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT()
            .setMessageSeverity(
                  vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
                | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
            )
            .setMessageType(
                  vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
                | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
            )
            .setPfnUserCallback(_VulkanDebugMessageCallback);

        // Allows debug messages during instance creation
        instanceCreateInfo.pNext = &debugUtilsMessengerCreateInfo;

    #endif

    Instance = vk::createInstance(instanceCreateInfo);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(Instance);

    #if defined(VK_EXT_debug_utils)
        _debugUtilsMessenger = Instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfo);
    #endif

    Log(DUSK_ANCHOR, "Vulkan Header Version: {}.{}.{}",
        VK_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE),
        VK_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE),
        VK_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE)
    );
}

void initSurface()
{
    if (!SDL_Vulkan_CreateSurface(_window, Instance, nullptr, reinterpret_cast<VkSurfaceKHR *>(&_surface))) {
        throw Exception("SDL_Vulkan_CreateSurface failed, {}", SDL_GetError());
    }
}

void initDevice()
{
    Log(DUSK_ANCHOR, "Available Physical Devices:");

    for (const auto& physicalDevice : Instance.enumeratePhysicalDevices()) {
        PhysicalDeviceProperties = physicalDevice.getProperties();
        PhysicalDeviceFeatures = physicalDevice.getFeatures();

        bool isSuitable = (
            PhysicalDeviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu
            // and _vkPhysicalDeviceFeatures.geometryShader
        );

        // isSuitable = StringView(PhysicalDeviceProperties.deviceName.data()).starts_with("llvmpipe");

        if (isSuitable and not _physicalDevice) {
            _physicalDevice = physicalDevice;
        }

        Log(DUSK_ANCHOR, " - {}", PhysicalDeviceProperties.deviceName.data());
    }

    if (not _physicalDevice) {
        throw Exception("No suitable physical device found");
    }

    Log(DUSK_ANCHOR, "Physical Device Name: {}", PhysicalDeviceProperties.deviceName.data());

    Log(DUSK_ANCHOR, "Physical Vulkan Version: {}.{}.{}",
        VK_VERSION_MAJOR(PhysicalDeviceProperties.apiVersion),
        VK_VERSION_MINOR(PhysicalDeviceProperties.apiVersion),
        VK_VERSION_PATCH(PhysicalDeviceProperties.apiVersion)
    );

    _graphicsQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    _presentQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    Log(DUSK_ANCHOR, "Available Vulkan Queue Families:");

    for (auto&& [index, properties] : Enumerate(_physicalDevice.getQueueFamilyProperties())) {
        bool hasPresent = _physicalDevice.getSurfaceSupportKHR(index, _surface);
        bool hasGraphics = bool(properties.queueFlags & vk::QueueFlagBits::eGraphics);

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

        Log(DUSK_ANCHOR, " - #{}: {} Queues, Present: {}, {}",
            index,
            properties.queueCount,
            (hasPresent ? "yes" : "no"),
            vk::to_string(properties.queueFlags)
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

    List<vk::DeviceQueueCreateInfo> queueCreateInfoList;
    for (auto index : queueFamilyIndexSet) {
        queueCreateInfoList.push_back(
            vk::DeviceQueueCreateInfo()
                .setQueueFamilyIndex(index)
                .setQueueCount(QUEUE_COUNT)
                .setPQueuePriorities(QUEUE_PRIORITIES)
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

    auto deviceCreateInfo = vk::DeviceCreateInfo()
        .setQueueCreateInfos(queueCreateInfoList)
        .setPEnabledExtensionNames(requiredDeviceExtensionList);

    Device = _physicalDevice.createDevice(deviceCreateInfo);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(Device);

    _graphicsQueue = Device.getQueue(_graphicsQueueFamilyIndex, 0);
    _presentQueue = Device.getQueue(_presentQueueFamilyIndex, 0);
}

void initAllocator()
{
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

    VkResult result = vmaCreateAllocator(&allocatorCreateInfo, &Allocator);
    vk::detail::resultCheck(vk::Result(result), "vmaCreateAllocator");

    auto memoryProperties = _physicalDevice.getMemoryProperties();

    List<VmaBudget> heapBudgetList(memoryProperties.memoryHeapCount);
    vmaGetHeapBudgets(Allocator, heapBudgetList.data());

    for (auto&& [heapIndex, heap] : Enumerate(heapBudgetList)) {
        Log(DUSK_ANCHOR, "Vulkan Memory Heap #{}: {}", heapIndex, FormatBytesHumanReadable(heap.budget));

        for (auto&& [typeIndex, type] : Enumerate(memoryProperties.memoryTypes)) {
            if (type.heapIndex == heapIndex and type.propertyFlags) {
                Log(DUSK_ANCHOR, " - Type #{}: {}", typeIndex, vk::to_string(type.propertyFlags));
            }
        }
    }
}

void initDepthBuffer()
{
    constexpr Array<vk::Format, 5> DEPTH_FORMAT_LIST = {
        vk::Format::eD32Sfloat,
        vk::Format::eD32SfloatS8Uint,
        vk::Format::eD24UnormS8Uint,
        vk::Format::eD16Unorm,
        vk::Format::eD16UnormS8Uint
    };

    if (_depthImageFormat == vk::Format::eUndefined) {
        for (const auto& format : DEPTH_FORMAT_LIST) {
            vk::FormatProperties properties = _physicalDevice.getFormatProperties(format);

            bool hasDepthStencil = bool(properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment);

            if (hasDepthStencil) {
                _depthImageFormat = format;
                break;
            }
        }

        if (_depthImageFormat == vk::Format::eUndefined) {
            throw Exception("Unable to find suitable depth buffer image format");
        }

        Log(DUSK_ANCHOR, "Vulkan Depth Buffer Image Format: {}", vk::to_string(_depthImageFormat));
    }

    Device.destroyImageView(_depthImageView);
    Device.destroyImage(_depthImage);
    vmaFreeMemory(Allocator, _depthImageAllocation);

    auto imageCreateInfo = vk::ImageCreateInfo()
        .setImageType(vk::ImageType::e2D)
        .setFormat(_depthImageFormat)
        .setExtent(vk::Extent3D(_swapchainExtent, 1))
        .setMipLevels(1)
        .setArrayLayers(1)
        .setTiling(vk::ImageTiling::eOptimal)
        .setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment)
        .setSharingMode(vk::SharingMode::eExclusive)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setInitialLayout(vk::ImageLayout::eUndefined)
        .setSharingMode(vk::SharingMode::eExclusive);

    auto allocationCreateInfo = VmaAllocationCreateInfo{
        .usage = VMA_MEMORY_USAGE_GPU_ONLY,
        .requiredFlags = VkMemoryPropertyFlags(vk::MemoryPropertyFlagBits::eDeviceLocal)
    };

    std::tie(_depthImage, _depthImageAllocation) = CreateImage(imageCreateInfo, allocationCreateInfo);

    auto imageViewCreateInfo = vk::ImageViewCreateInfo()
        .setImage(_depthImage)
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(_depthImageFormat)
        .setSubresourceRange(vk::ImageSubresourceRange(
            vk::ImageAspectFlagBits::eDepth,
            0,
            1,
            0,
            1
        ));

    _depthImageView = Device.createImageView(imageViewCreateInfo);
}

void initRenderPass()
{
    auto colorAttachmentDescription = vk::AttachmentDescription()
        .setFormat(_swapchainImageFormat)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setInitialLayout(vk::ImageLayout::eUndefined)
        .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

    auto depthAttachmentDescription = vk::AttachmentDescription()
        .setFormat(_depthImageFormat)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setInitialLayout(vk::ImageLayout::eUndefined)
        .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    auto colorAttachmentReference = vk::AttachmentReference()
        .setAttachment(0)
        .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    auto depthAttachmentReference = vk::AttachmentReference()
        .setAttachment(1)
        .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    auto subpassDescription = vk::SubpassDescription()
        .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
        .setColorAttachmentCount(1)
        .setPColorAttachments(&colorAttachmentReference)
        .setPDepthStencilAttachment(&depthAttachmentReference);

    Array<vk::SubpassDependency, 1> subpassDependencyList = {
        vk::SubpassDependency()
            .setSrcSubpass(VK_SUBPASS_EXTERNAL)
            .setDstSubpass(0)
            .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eLateFragmentTests)
            .setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
            .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
            .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite),
        // vk::SubpassDependency()
        //     .setSrcSubpass(VK_SUBPASS_EXTERNAL)
        //     .setDstSubpass(0)
        //     .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        //     .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        //     .setSrcAccessMask({})
        //     .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite),
        // vk::SubpassDependency()
        //     .setSrcSubpass(VK_SUBPASS_EXTERNAL)
        //     .setDstSubpass(0)
        //     .setSrcStageMask(
        //         vk::PipelineStageFlagBits::eEarlyFragmentTests |
        //         vk::PipelineStageFlagBits::eLateFragmentTests
        //     )
        //     .setDstStageMask(
        //         vk::PipelineStageFlagBits::eEarlyFragmentTests |
        //         vk::PipelineStageFlagBits::eLateFragmentTests
        //     )
        //     .setSrcAccessMask({})
        //     .setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite),
    };

    Array<vk::AttachmentDescription, 2> attachmentList = {
        colorAttachmentDescription,
        depthAttachmentDescription,
    };

    Device.destroyRenderPass(RenderPass);

    auto renderPassCreateInfo = vk::RenderPassCreateInfo()
        .setAttachments(attachmentList)
        .setSubpasses({ subpassDescription })
        .setDependencies(subpassDependencyList);

    RenderPass = Device.createRenderPass(renderPassCreateInfo);
}

void initDescriptorPool()
{
    ///
    /// Descriptor Pool
    ///

    Array<vk::DescriptorPoolSize, 1> descriptorPoolSizeList = {
        vk::DescriptorPoolSize()
            .setType(vk::DescriptorType::eUniformBuffer)
            .setDescriptorCount(1000), // TODO: Improve
    };

    auto descriptorPoolCreateInfo = vk::DescriptorPoolCreateInfo()
        .setMaxSets(1000)
        .setPoolSizes(descriptorPoolSizeList);

    DescriptorPool = Device.createDescriptorPool(descriptorPoolCreateInfo);
}

void initDescriptorSetLayout()
{

    Array<vk::DescriptorSetLayoutBinding, 2> _descriptorSetLayoutBindingList = {
        vk::DescriptorSetLayoutBinding()
            .setBinding(ShaderGlobals::Binding)
            .setDescriptorType(vk::DescriptorType::eUniformBuffer)
            .setDescriptorCount(1)
            .setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment),
        vk::DescriptorSetLayoutBinding()
            .setBinding(ShaderTransform::Binding)
            .setDescriptorType(vk::DescriptorType::eUniformBuffer)
            .setDescriptorCount(1)
            .setStageFlags(vk::ShaderStageFlagBits::eVertex),
    };

    // VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    // VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER

    auto _descriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo()
        .setBindings(_descriptorSetLayoutBindingList);

    DescriptorSetLayout = Device.createDescriptorSetLayout(_descriptorSetLayoutCreateInfo);

    GlobalsBuffer.reset(new VulkanBuffer());
    GlobalsBuffer->Create(
        vk::BufferUsageFlagBits::eUniformBuffer,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        sizeof(ShaderGlobals)
    );
}

void initPipelineLayout()
{
    auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()
        .setSetLayouts({ DescriptorSetLayout });

    PipelineLayout = Graphics::Device.createPipelineLayout(pipelineLayoutCreateInfo);
}

void initFramebuffers()
{
    for (auto& framebuffer : _framebufferList) {
        Device.destroyFramebuffer(framebuffer);
        framebuffer = VK_NULL_HANDLE;
    }

    _framebufferList.resize(_swapchainImageViewList.size());

    for (auto&& [index, imageView] : Enumerate(_swapchainImageViewList)) {
        Array<vk::ImageView, 2> attachmentList = {
            imageView,
            _depthImageView,
        };

        auto framebufferCreateInfo = vk::FramebufferCreateInfo()
            .setRenderPass(RenderPass)
            .setAttachments(attachmentList)
            .setWidth(_swapchainExtent.width)
            .setHeight(_swapchainExtent.height)
            .setLayers(1);

        _framebufferList[index] = Device.createFramebuffer(framebufferCreateInfo);
    }
}

void initCommandBuffers()
{
    if (not _commandBufferList.empty()) {
        Device.freeCommandBuffers(_commandPool, _commandBufferList);
    }

    Device.destroyCommandPool(_commandPool);

    auto commandPoolCreateInfo = vk::CommandPoolCreateInfo()
        .setQueueFamilyIndex(_graphicsQueueFamilyIndex)
        .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

    _commandPool = Device.createCommandPool(commandPoolCreateInfo);

    auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
        .setCommandPool(_commandPool)
        .setLevel(vk::CommandBufferLevel::ePrimary)
        .setCommandBufferCount(_swapchainImageList.size());

    _commandBufferList = Device.allocateCommandBuffers(commandBufferAllocateInfo);
}

std::function<void(vk::CommandBuffer)> _renderCallback;

void fillCommandBuffers()
{
    for (auto&& [index, commandBuffer] : Enumerate(_commandBufferList)) {
        auto commandBufferBeginInfo = vk::CommandBufferBeginInfo();
        commandBuffer.begin(commandBufferBeginInfo);

        vec4 clearColor = Color::CornflowerBlue;

        // If our surface is sRGB, Vulkan will try to convert our color to sRGB
        // This fails and washes out the color, so we convert to linear to account for it  
        if (_swapchainColorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            clearColor = Color::ToLinear(clearColor);
        }

        Array<vk::ClearValue, 2> clearValueList = {
            vk::ClearValue(vk::ClearColorValue(Color::ToArray(clearColor))),
            vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0.0f)),
        };

        auto renderArea = vk::Rect2D()
            .setOffset({ 0, 0 })
            .setExtent(_swapchainExtent);

        auto renderPassBeginInfo = vk::RenderPassBeginInfo()
            .setRenderPass(RenderPass)
            .setFramebuffer(_framebufferList[index])
            .setRenderArea(renderArea)
            .setClearValues(clearValueList);

        commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        vk::Viewport viewport = {
            0,
            0,
            float(_swapchainExtent.width),
            float(_swapchainExtent.height),
            0.0f,
            1.0f
        };
        commandBuffer.setViewport(0, 1, &viewport);

        vk::Rect2D scissor = {
            { 0, 0 },
            _swapchainExtent,
        };
        commandBuffer.setScissor(0, 1, &scissor);

        if (_renderCallback) {
            _renderCallback(commandBuffer);
        }

        commandBuffer.endRenderPass();

        commandBuffer.end();
    }
}

void initSyncObjects()
{
    for (auto& fence : _inFlightFenceList) {
        Device.destroyFence(fence);
        fence = VK_NULL_HANDLE;
    }

    for (auto& semaphore : _imageAvailableSemaphoreList) {
        Device.destroySemaphore(semaphore);
        semaphore = VK_NULL_HANDLE;
    }

    for (auto& semaphore : _renderingFinishedSemaphoreList) {
        Device.destroySemaphore(semaphore);
        semaphore = VK_NULL_HANDLE;
    }

    size_t backbufferCount = _swapchainImageViewList.size();

    _inFlightFenceList.resize(backbufferCount);
    _imageAvailableSemaphoreList.resize(backbufferCount);
    _renderingFinishedSemaphoreList.resize(backbufferCount);

    auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();

    auto fenceCreateInfo = vk::FenceCreateInfo()
        .setFlags(vk::FenceCreateFlagBits::eSignaled);

    for (size_t i = 0; i < backbufferCount; ++i) {
        _inFlightFenceList[i] = Device.createFence(fenceCreateInfo);
        _imageAvailableSemaphoreList[i] = Device.createSemaphore(semaphoreCreateInfo);
        _renderingFinishedSemaphoreList[i] = Device.createSemaphore(semaphoreCreateInfo);
    }
}

void initSwapchain()
{
    Device.waitIdle();

    // Image Format

    if (_swapchainImageFormat == vk::Format::eUndefined) {
        auto surfaceFormatList = _physicalDevice.getSurfaceFormatsKHR(_surface);
        if (surfaceFormatList.empty()) {
            throw Exception("No surface formats available");
        }

        Log(DUSK_ANCHOR, "Available Vulkan Surface Formats:");
        for (const auto& surfaceFormat : surfaceFormatList) {
            bool isPreferredFormat = (
                   surfaceFormat.format == vk::Format::eR8G8B8A8Srgb
                or surfaceFormat.format == vk::Format::eB8G8R8A8Srgb
            );

            bool isSRGB = (surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear);

            if (isPreferredFormat and isSRGB) {
                _swapchainImageFormat = surfaceFormat.format;
                _swapchainColorSpace = surfaceFormat.colorSpace;
            }

            Log(DUSK_ANCHOR, " - {} {}",
                vk::to_string(surfaceFormat.format),
                vk::to_string(surfaceFormat.colorSpace)
            );
        }

        if (_swapchainImageFormat == vk::Format::eUndefined) {
            Log(DUSK_ANCHOR, "Unable to find a preferred surface format");

            const auto& surfaceFormat = surfaceFormatList.front();
            _swapchainImageFormat = surfaceFormat.format;
            _swapchainColorSpace = surfaceFormat.colorSpace;
        }

        Log(DUSK_ANCHOR, "Vulkan Swapchain Image Format: {} {}",
            vk::to_string(_swapchainImageFormat),
            vk::to_string(_swapchainColorSpace)
        );
    }

    // Image Extent

    const auto& surfaceCapabilities = _physicalDevice.getSurfaceCapabilitiesKHR(_surface);

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
        _swapchainPresentMode = vk::PresentModeKHR::eFifo;

        const auto& presentModeList = _physicalDevice.getSurfacePresentModesKHR(_surface);
        
        Log(DUSK_ANCHOR, "Available Vulkan Present Modes:");
        for (const auto& mode : presentModeList) {
            Log(DUSK_ANCHOR, " - {}", vk::to_string(mode));
        }

        // Mailbox is like FIFO, but automatically discards extra images
        if (ListContains(presentModeList, vk::PresentModeKHR::eMailbox)) {
            _swapchainPresentMode = vk::PresentModeKHR::eMailbox;
        }

        _swapchainPresentModeUndefined = false;
    }

    Log(DUSK_ANCHOR, "Vulkan Swapchain Present Mode: {}", vk::to_string(_swapchainPresentMode));

    // Actual

    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 and imageCount > surfaceCapabilities.maxImageCount) {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    auto oldSwapchain = _swapchain;

    auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR()
        .setSurface(_surface)
        .setMinImageCount(imageCount)
        .setImageFormat(_swapchainImageFormat)
        .setImageColorSpace(_swapchainColorSpace)
        .setImageExtent(_swapchainExtent)
        .setImageArrayLayers(1) // Always 1 for 3D applications
        .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
        .setPreTransform(surfaceCapabilities.currentTransform)
        .setPresentMode(_swapchainPresentMode)
        .setClipped(true)
        .setOldSwapchain(oldSwapchain);

    swapchainCreateInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);

    // if (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) {
    //     swapchainCreateInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::ePreMultiplied);
    // }
    // else if (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) {
    //     swapchainCreateInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::ePostMultiplied);
    // }
    // else if (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) {
    //     swapchainCreateInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eInherit);
    // }

    Array<uint32_t, 2> queueFamilyIndexList = {
        _graphicsQueueFamilyIndex,
        _presentQueueFamilyIndex,
    };

    if (_graphicsQueueFamilyIndex != _presentQueueFamilyIndex) {
        swapchainCreateInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
        swapchainCreateInfo.setQueueFamilyIndices(queueFamilyIndexList);
    }
    else {
        swapchainCreateInfo.setImageSharingMode(vk::SharingMode::eExclusive);
    }

    Log(DUSK_ANCHOR, "Vulkan Swapchain Image Count: {}", swapchainCreateInfo.minImageCount);
    Log(DUSK_ANCHOR, "Vulkan Swapchain Pre-Transform: {}", vk::to_string(swapchainCreateInfo.preTransform));
    Log(DUSK_ANCHOR, "Vulkan Swapchain Composite Alpha: {}", vk::to_string(swapchainCreateInfo.compositeAlpha));
    Log(DUSK_ANCHOR, "Vulkan Swapchain Sharing Mode: {}", vk::to_string(swapchainCreateInfo.imageSharingMode));

    _swapchain = Device.createSwapchainKHR(swapchainCreateInfo);

    Device.destroySwapchainKHR(oldSwapchain);

    // Image List

    _swapchainImageList = Device.getSwapchainImagesKHR(_swapchain);

    for (auto& imageView : _swapchainImageViewList) {
        Device.destroyImageView(imageView);
    }

    _swapchainImageViewList.resize(_swapchainImageList.size(), VK_NULL_HANDLE);

    auto imageViewCreateInfo = vk::ImageViewCreateInfo()
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(_swapchainImageFormat)
        .setSubresourceRange({ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 });

    for (const auto&& [index, image] : Enumerate(_swapchainImageList)) {
        imageViewCreateInfo.setImage(image);
        _swapchainImageViewList[index] = Device.createImageView(imageViewCreateInfo);
    }

    initDepthBuffer();
    initRenderPass();
    initFramebuffers();
    initCommandBuffers();
    fillCommandBuffers(); // TODO: Improve
    initSyncObjects();
}

DUSK_API
void SetRenderCallback(std::function<void(vk::CommandBuffer)> renderCallback)
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
    Device.waitIdle();

    // Sync Objects

    for (auto fence : _inFlightFenceList) {
        Device.destroyFence(fence);
        fence = nullptr;
    }

    for (auto semaphore : _renderingFinishedSemaphoreList) {
        Device.destroySemaphore(semaphore);
        semaphore = nullptr;
    }

    for (auto semaphore : _imageAvailableSemaphoreList) {
        Device.destroySemaphore(semaphore);
        semaphore = nullptr;
    }

    // Framebuffers

    for (auto& framebuffer : _framebufferList) {
        Device.destroyFramebuffer(framebuffer);
        framebuffer = VK_NULL_HANDLE;
    }

    // Command Buffers

    Device.freeCommandBuffers(_commandPool, _commandBufferList);

    Device.destroyCommandPool(_commandPool);

    // Render Pass

    Device.destroyRenderPass(RenderPass);

    // Depth Buffer

    Device.destroyImageView(_depthImageView);

    Device.destroyImage(_depthImage);

    vmaFreeMemory(Allocator, _depthImageAllocation);

    // Swapchain

    for (auto& imageView : _swapchainImageViewList) {
        Device.destroyImageView(imageView);
        imageView = VK_NULL_HANDLE;
    }

    Device.destroySwapchainKHR(_swapchain);

    // Pipeline Layout

    Device.destroyPipelineLayout(PipelineLayout);

    // Descriptor Set Layouts

    GlobalsBuffer->Destroy();

    Device.destroyDescriptorSetLayout(DescriptorSetLayout);

    // Descriptor Pools

    Device.destroyDescriptorPool(DescriptorPool);

    // Allocator

    vmaDestroyAllocator(Allocator);

    // Device

    Device.destroy();

    // Surface

    Instance.destroySurfaceKHR(_surface);

    // Instance

    Instance.destroyDebugUtilsMessengerEXT(_debugUtilsMessenger);

    Instance.destroy();

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
    static bool updateSwapchain = false;

    if (updateSwapchain) {
        Log(DUSK_ANCHOR, "Regenerating Swapchain");
        initSwapchain();
        updateSwapchain = false;
    }

    vk::Result result;

    vmaSetCurrentFrameIndex(Allocator, _currentFrame);

    result = Device.waitForFences(1, &_inFlightFenceList[_currentFrame], true, MAX_TIMEOUT);
    vk::detail::resultCheck(result, "vk::Device::waitForFences");

    result = Device.resetFences(1, &_inFlightFenceList[_currentFrame]);


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

    try {
        std::tie(result, imageIndex) = Device.acquireNextImageKHR(
            _swapchain,
            MAX_TIMEOUT,
            _imageAvailableSemaphoreList[_currentFrame],
            VK_NULL_HANDLE
        );
    }
    catch (vk::OutOfDateKHRError & e) {
        updateSwapchain = true;
        return;
    }

    Array<vk::Semaphore, 1> waitSemaphoreList = {
        _imageAvailableSemaphoreList[_currentFrame],
    };

    Array<vk::PipelineStageFlags, 1> waitStageList = {
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
    };

    Array<vk::CommandBuffer, 1> commandBufferList = {
        _commandBufferList[imageIndex],
    };

    Array<vk::Semaphore, 1> signalSemaphoreList = {
        _renderingFinishedSemaphoreList[_currentFrame],
    };

    auto submitInfo = vk::SubmitInfo()
        .setWaitSemaphores(waitSemaphoreList)
        .setWaitDstStageMask(waitStageList)
        .setCommandBuffers(commandBufferList)
        .setSignalSemaphores(signalSemaphoreList);

    _graphicsQueue.submit(submitInfo, _inFlightFenceList[_currentFrame]);

    Array<vk::SwapchainKHR, 1> swapchainList = {
        _swapchain,
    };

    Array<uint32_t, 1> imageIndexList = {
        imageIndex,
    };

    auto presentInfo = vk::PresentInfoKHR()
        .setSwapchains(swapchainList)
        .setWaitSemaphores(signalSemaphoreList)
        .setImageIndices(imageIndexList);

    try {
        result = _presentQueue.presentKHR(presentInfo);
    }
    catch (vk::OutOfDateKHRError & e) {
        updateSwapchain = true;
        return;
    }

    _currentFrame = (_currentFrame + 1) % _inFlightFenceList.size();

    // TODO: Fix
    Device.waitIdle();
    fillCommandBuffers();
}

DUSK_API
void HandleEvent(SDL_Event * event)
{
    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        _windowSize.setWidth(event->window.data1);
        _windowSize.setHeight(event->window.data2);
    }
}

DUSK_API
vk::Extent2D GetWindowSize()
{
    return _windowSize;
}

DUSK_API
Tuple<vk::Buffer, VmaAllocation> CreateBuffer(
    vk::BufferCreateInfo& bufferCreateInfo,
    VmaAllocationCreateInfo& allocationCreateInfo,
    VmaAllocationInfo * allocationInfo /* = nullptr */
)
{
    vk::Buffer buffer;
    VmaAllocation allocation;

    auto result = vmaCreateBuffer(
        Allocator,
        reinterpret_cast<const VkBufferCreateInfo *>(&bufferCreateInfo),
        &allocationCreateInfo,
        reinterpret_cast<VkBuffer *>(&buffer),
        &allocation,
        allocationInfo
    );

    vk::detail::resultCheck(vk::Result(result), "vmaCreateBuffer");

    return { buffer, allocation };
}

Tuple<vk::Image, VmaAllocation> CreateImage(
    vk::ImageCreateInfo& imageCreateInfo,
    VmaAllocationCreateInfo& allocationCreateInfo,
    VmaAllocationInfo * allocationInfo /* = nullptr */
)
{
    vk::Image image;
    VmaAllocation allocation;

    auto result = vmaCreateImage(
        Allocator,
        reinterpret_cast<const VkImageCreateInfo *>(&imageCreateInfo),
        &allocationCreateInfo,
        reinterpret_cast<VkImage *>(&image),
        &allocation,
        allocationInfo
    );

    vk::detail::resultCheck(vk::Result(result), "vmaCreateImage");

    return { image, allocation };
}

DUSK_API
void CopyBuffer(vk::Buffer source, vk::Buffer destination, vk::BufferCopy region)
{
    auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
        .setCommandPool(_commandPool)
        .setLevel(vk::CommandBufferLevel::ePrimary)
        .setCommandBufferCount(1);

    auto commandBufferList = Device.allocateCommandBuffers(commandBufferAllocateInfo);
    auto commandBuffer = commandBufferList.front();

    auto commandBufferBeginInfo = vk::CommandBufferBeginInfo()
        .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

    commandBuffer.begin(commandBufferBeginInfo);

    commandBuffer.copyBuffer(source, destination, { region });

    commandBuffer.end();

    auto submitInfo = vk::SubmitInfo()
        .setCommandBuffers(commandBufferList);

    _graphicsQueue.submit({ submitInfo }, nullptr);

    _graphicsQueue.waitIdle();

    Device.freeCommandBuffers(_commandPool, commandBufferList);
}

DUSK_API
void CopyBufferToImage(vk::Buffer source, vk::Image destination, vk::BufferImageCopy region)
{
    auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
        .setCommandPool(_commandPool)
        .setLevel(vk::CommandBufferLevel::ePrimary)
        .setCommandBufferCount(1);

    auto commandBufferList = Device.allocateCommandBuffers(commandBufferAllocateInfo);
    auto commandBuffer = commandBufferList.front();

    auto commandBufferBeginInfo = vk::CommandBufferBeginInfo()
        .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

    auto subresourceRange = vk::ImageSubresourceRange()
        .setAspectMask(region.imageSubresource.aspectMask)
        .setBaseMipLevel(0)
        .setLevelCount(region.imageSubresource.mipLevel + 1)
        .setBaseArrayLayer(region.imageSubresource.baseArrayLayer)
        .setLayerCount(region.imageSubresource.layerCount);

    auto barrier = vk::ImageMemoryBarrier()
        .setOldLayout(vk::ImageLayout::eUndefined)
        .setNewLayout(vk::ImageLayout::eTransferDstOptimal)
        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setImage(destination)
        .setSubresourceRange(subresourceRange)
        .setSrcAccessMask({})
        .setDstAccessMask(vk::AccessFlagBits::eTransferWrite);

    commandBuffer.begin(commandBufferBeginInfo);

    commandBuffer.pipelineBarrier(
        vk::PipelineStageFlagBits::eTopOfPipe,
        vk::PipelineStageFlagBits::eTransfer,
        {},
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    commandBuffer.copyBufferToImage(
        source,
        destination,
        vk::ImageLayout::eTransferDstOptimal,
        { region }
    );

    commandBuffer.end();

    auto submitInfo = vk::SubmitInfo()
        .setCommandBuffers(commandBufferList);

    _graphicsQueue.submit({ submitInfo }, nullptr);

    _graphicsQueue.waitIdle();

    Device.freeCommandBuffers(_commandPool, commandBufferList);
}

} // namespace Graphics
} // namespace dusk