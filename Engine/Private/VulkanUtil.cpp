#include <Dusk/VulkanUtil.hpp>

namespace dusk {

StringView VkResultToString(VkResult result)
{
    switch (result) {
    case VK_SUCCESS:
        return "VK_SUCCESS";
    case VK_NOT_READY:
        return "VK_NOT_READY";
    case VK_TIMEOUT:
        return "VK_TIMEOUT";
    case VK_EVENT_SET:
        return "VK_EVENT_SET";
    case VK_EVENT_RESET:
        return "VK_EVENT_RESET";
    case VK_INCOMPLETE:
        return "VK_INCOMPLETE";
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case VK_ERROR_INITIALIZATION_FAILED:
        return "VK_ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_DEVICE_LOST:
        return "VK_ERROR_DEVICE_LOST";
    case VK_ERROR_MEMORY_MAP_FAILED:
        return "VK_ERROR_MEMORY_MAP_FAILED";
    case VK_ERROR_LAYER_NOT_PRESENT:
        return "VK_ERROR_LAYER_NOT_PRESENT";
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT:
        return "VK_ERROR_FEATURE_NOT_PRESENT";
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_TOO_MANY_OBJECTS:
        return "VK_ERROR_TOO_MANY_OBJECTS";
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    case VK_ERROR_FRAGMENTED_POOL:
        return "VK_ERROR_FRAGMENTED_POOL";
    case VK_ERROR_UNKNOWN:
        return "VK_ERROR_UNKNOWN";
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        return "VK_ERROR_OUT_OF_POOL_MEMORY";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
        return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    case VK_ERROR_FRAGMENTATION:
        return "VK_ERROR_FRAGMENTATION";
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
        return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
    case VK_PIPELINE_COMPILE_REQUIRED:
        return "VK_PIPELINE_COMPILE_REQUIRED";
    case VK_ERROR_SURFACE_LOST_KHR:
        return "VK_ERROR_SURFACE_LOST_KHR";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case VK_SUBOPTIMAL_KHR:
        return "VK_SUBOPTIMAL_KHR";
    case VK_ERROR_OUT_OF_DATE_KHR:
        return "VK_ERROR_OUT_OF_DATE_KHR";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case VK_ERROR_VALIDATION_FAILED_EXT:
        return "VK_ERROR_VALIDATION_FAILED_EXT";
    case VK_ERROR_INVALID_SHADER_NV:
        return "VK_ERROR_INVALID_SHADER_NV";
    case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
        return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
        return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
        return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
        return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
        return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
        return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
        return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
    case VK_ERROR_NOT_PERMITTED_KHR:
        return "VK_ERROR_NOT_PERMITTED_KHR";
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
    case VK_THREAD_IDLE_KHR:
        return "VK_THREAD_IDLE_KHR";
    case VK_THREAD_DONE_KHR:
        return "VK_THREAD_DONE_KHR";
    case VK_OPERATION_DEFERRED_KHR:
        return "VK_OPERATION_DEFERRED_KHR";
    case VK_OPERATION_NOT_DEFERRED_KHR:
        return "VK_OPERATION_NOT_DEFERRED_KHR";
    case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
        return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
    case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
        return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
    case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
        return "VK_INCOMPATIBLE_SHADER_BINARY_EXT";
    default:
        return "Invalid VkResult";
    }
}

StringView VkObjectTypeToString(VkObjectType objectType)
{
    switch (objectType) {
    case VK_OBJECT_TYPE_UNKNOWN:
        return "VK_OBJECT_TYPE_UNKNOWN";
    case VK_OBJECT_TYPE_INSTANCE:
        return "VK_OBJECT_TYPE_INSTANCE";
    case VK_OBJECT_TYPE_PHYSICAL_DEVICE:
        return "VK_OBJECT_TYPE_PHYSICAL_DEVICE";
    case VK_OBJECT_TYPE_DEVICE:
        return "VK_OBJECT_TYPE_DEVICE";
    case VK_OBJECT_TYPE_QUEUE:
        return "VK_OBJECT_TYPE_QUEUE";
    case VK_OBJECT_TYPE_SEMAPHORE:
        return "VK_OBJECT_TYPE_SEMAPHORE";
    case VK_OBJECT_TYPE_COMMAND_BUFFER:
        return "VK_OBJECT_TYPE_COMMAND_BUFFER";
    case VK_OBJECT_TYPE_FENCE:
        return "VK_OBJECT_TYPE_FENCE";
    case VK_OBJECT_TYPE_DEVICE_MEMORY:
        return "VK_OBJECT_TYPE_DEVICE_MEMORY";
    case VK_OBJECT_TYPE_BUFFER:
        return "VK_OBJECT_TYPE_BUFFER";
    case VK_OBJECT_TYPE_IMAGE:
        return "VK_OBJECT_TYPE_IMAGE";
    case VK_OBJECT_TYPE_EVENT:
        return "VK_OBJECT_TYPE_EVENT";
    case VK_OBJECT_TYPE_QUERY_POOL:
        return "VK_OBJECT_TYPE_QUERY_POOL";
    case VK_OBJECT_TYPE_BUFFER_VIEW:
        return "VK_OBJECT_TYPE_BUFFER_VIEW";
    case VK_OBJECT_TYPE_IMAGE_VIEW:
        return "VK_OBJECT_TYPE_IMAGE_VIEW";
    case VK_OBJECT_TYPE_SHADER_MODULE:
        return "VK_OBJECT_TYPE_SHADER_MODULE";
    case VK_OBJECT_TYPE_PIPELINE_CACHE:
        return "VK_OBJECT_TYPE_PIPELINE_CACHE";
    case VK_OBJECT_TYPE_PIPELINE_LAYOUT:
        return "VK_OBJECT_TYPE_PIPELINE_LAYOUT";
    case VK_OBJECT_TYPE_RENDER_PASS:
        return "VK_OBJECT_TYPE_RENDER_PASS";
    case VK_OBJECT_TYPE_PIPELINE:
        return "VK_OBJECT_TYPE_PIPELINE";
    case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:
        return "VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT";
    case VK_OBJECT_TYPE_SAMPLER:
        return "VK_OBJECT_TYPE_SAMPLER";
    case VK_OBJECT_TYPE_DESCRIPTOR_POOL:
        return "VK_OBJECT_TYPE_DESCRIPTOR_POOL";
    case VK_OBJECT_TYPE_DESCRIPTOR_SET:
        return "VK_OBJECT_TYPE_DESCRIPTOR_SET";
    case VK_OBJECT_TYPE_FRAMEBUFFER:
        return "VK_OBJECT_TYPE_FRAMEBUFFER";
    case VK_OBJECT_TYPE_COMMAND_POOL:
        return "VK_OBJECT_TYPE_COMMAND_POOL";
    case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:
        return "VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION";
    case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:
        return "VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE";
    case VK_OBJECT_TYPE_PRIVATE_DATA_SLOT:
        return "VK_OBJECT_TYPE_PRIVATE_DATA_SLOT";
    case VK_OBJECT_TYPE_SURFACE_KHR:
        return "VK_OBJECT_TYPE_SURFACE_KHR";
    case VK_OBJECT_TYPE_SWAPCHAIN_KHR:
        return "VK_OBJECT_TYPE_SWAPCHAIN_KHR";
    case VK_OBJECT_TYPE_DISPLAY_KHR:
        return "VK_OBJECT_TYPE_DISPLAY_KHR";
    case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:
        return "VK_OBJECT_TYPE_DISPLAY_MODE_KHR";
    case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:
        return "VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT";
    case VK_OBJECT_TYPE_VIDEO_SESSION_KHR:
        return "VK_OBJECT_TYPE_VIDEO_SESSION_KHR";
    case VK_OBJECT_TYPE_VIDEO_SESSION_PARAMETERS_KHR:
        return "VK_OBJECT_TYPE_VIDEO_SESSION_PARAMETERS_KHR";
    case VK_OBJECT_TYPE_CU_MODULE_NVX:
        return "VK_OBJECT_TYPE_CU_MODULE_NVX";
    case VK_OBJECT_TYPE_CU_FUNCTION_NVX:
        return "VK_OBJECT_TYPE_CU_FUNCTION_NVX";
    case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:
        return "VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT";
    case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR:
        return "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR";
    case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT:
        return "VK_OBJECT_TYPE_VALIDATION_CACHE_EXT";
    case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV:
        return "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV";
    case VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL:
        return "VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL";
    case VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR:
        return "VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR";
    case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV:
        return "VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV";
    case VK_OBJECT_TYPE_CUDA_MODULE_NV:
        return "VK_OBJECT_TYPE_CUDA_MODULE_NV";
    case VK_OBJECT_TYPE_CUDA_FUNCTION_NV:
        return "VK_OBJECT_TYPE_CUDA_FUNCTION_NV";
    case VK_OBJECT_TYPE_BUFFER_COLLECTION_FUCHSIA:
        return "VK_OBJECT_TYPE_BUFFER_COLLECTION_FUCHSIA";
    case VK_OBJECT_TYPE_MICROMAP_EXT:
        return "VK_OBJECT_TYPE_MICROMAP_EXT";
    case VK_OBJECT_TYPE_OPTICAL_FLOW_SESSION_NV:
        return "VK_OBJECT_TYPE_OPTICAL_FLOW_SESSION_NV";
    case VK_OBJECT_TYPE_SHADER_EXT:
        return "VK_OBJECT_TYPE_SHADER_EXT";
    default:
        return "Invalid VkObjectType";
    }
}

StringView VkSampleCountFlagBitsToString(VkSampleCountFlagBits sampleCount)
{
    switch (sampleCount) {
    case VK_SAMPLE_COUNT_1_BIT:
        return "VK_SAMPLE_COUNT_1_BIT";
    case VK_SAMPLE_COUNT_2_BIT:
        return "VK_SAMPLE_COUNT_2_BIT";
    case VK_SAMPLE_COUNT_4_BIT:
        return "VK_SAMPLE_COUNT_4_BIT";
    case VK_SAMPLE_COUNT_8_BIT:
        return "VK_SAMPLE_COUNT_8_BIT";
    case VK_SAMPLE_COUNT_16_BIT:
        return "VK_SAMPLE_COUNT_16_BIT";
    case VK_SAMPLE_COUNT_32_BIT:
        return "VK_SAMPLE_COUNT_32_BIT";
    case VK_SAMPLE_COUNT_64_BIT:
        return "VK_SAMPLE_COUNT_64_BIT";
    default:
        return "Invalid VkSampleCountFlagBits";
    }
}

StringView VkFormatToString(VkFormat format)
{
    switch (format) {
    case VK_FORMAT_UNDEFINED:
        return "VK_FORMAT_UNDEFINED";
    case VK_FORMAT_R4G4_UNORM_PACK8:
        return "VK_FORMAT_R4G4_UNORM_PACK8";
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
        return "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
        return "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
    case VK_FORMAT_R5G6B5_UNORM_PACK16:
        return "VK_FORMAT_R5G6B5_UNORM_PACK16";
    case VK_FORMAT_B5G6R5_UNORM_PACK16:
        return "VK_FORMAT_B5G6R5_UNORM_PACK16";
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
        return "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
        return "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
        return "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
    case VK_FORMAT_R8_UNORM:
        return "VK_FORMAT_R8_UNORM";
    case VK_FORMAT_R8_SNORM:
        return "VK_FORMAT_R8_SNORM";
    case VK_FORMAT_R8_USCALED:
        return "VK_FORMAT_R8_USCALED";
    case VK_FORMAT_R8_SSCALED:
        return "VK_FORMAT_R8_SSCALED";
    case VK_FORMAT_R8_UINT:
        return "VK_FORMAT_R8_UINT";
    case VK_FORMAT_R8_SINT:
        return "VK_FORMAT_R8_SINT";
    case VK_FORMAT_R8_SRGB:
        return "VK_FORMAT_R8_SRGB";
    case VK_FORMAT_R8G8_UNORM:
        return "VK_FORMAT_R8G8_UNORM";
    case VK_FORMAT_R8G8_SNORM:
        return "VK_FORMAT_R8G8_SNORM";
    case VK_FORMAT_R8G8_USCALED:
        return "VK_FORMAT_R8G8_USCALED";
    case VK_FORMAT_R8G8_SSCALED:
        return "VK_FORMAT_R8G8_SSCALED";
    case VK_FORMAT_R8G8_UINT:
        return "VK_FORMAT_R8G8_UINT";
    case VK_FORMAT_R8G8_SINT:
        return "VK_FORMAT_R8G8_SINT";
    case VK_FORMAT_R8G8_SRGB:
        return "VK_FORMAT_R8G8_SRGB";
    case VK_FORMAT_R8G8B8_UNORM:
        return "VK_FORMAT_R8G8B8_UNORM";
    case VK_FORMAT_R8G8B8_SNORM:
        return "VK_FORMAT_R8G8B8_SNORM";
    case VK_FORMAT_R8G8B8_USCALED:
        return "VK_FORMAT_R8G8B8_USCALED";
    case VK_FORMAT_R8G8B8_SSCALED:
        return "VK_FORMAT_R8G8B8_SSCALED";
    case VK_FORMAT_R8G8B8_UINT:
        return "VK_FORMAT_R8G8B8_UINT";
    case VK_FORMAT_R8G8B8_SINT:
        return "VK_FORMAT_R8G8B8_SINT";
    case VK_FORMAT_R8G8B8_SRGB:
        return "VK_FORMAT_R8G8B8_SRGB";
    case VK_FORMAT_B8G8R8_UNORM:
        return "VK_FORMAT_B8G8R8_UNORM";
    case VK_FORMAT_B8G8R8_SNORM:
        return "VK_FORMAT_B8G8R8_SNORM";
    case VK_FORMAT_B8G8R8_USCALED:
        return "VK_FORMAT_B8G8R8_USCALED";
    case VK_FORMAT_B8G8R8_SSCALED:
        return "VK_FORMAT_B8G8R8_SSCALED";
    case VK_FORMAT_B8G8R8_UINT:
        return "VK_FORMAT_B8G8R8_UINT";
    case VK_FORMAT_B8G8R8_SINT:
        return "VK_FORMAT_B8G8R8_SINT";
    case VK_FORMAT_B8G8R8_SRGB:
        return "VK_FORMAT_B8G8R8_SRGB";
    case VK_FORMAT_R8G8B8A8_UNORM:
        return "VK_FORMAT_R8G8B8A8_UNORM";
    case VK_FORMAT_R8G8B8A8_SNORM:
        return "VK_FORMAT_R8G8B8A8_SNORM";
    case VK_FORMAT_R8G8B8A8_USCALED:
        return "VK_FORMAT_R8G8B8A8_USCALED";
    case VK_FORMAT_R8G8B8A8_SSCALED:
        return "VK_FORMAT_R8G8B8A8_SSCALED";
    case VK_FORMAT_R8G8B8A8_UINT:
        return "VK_FORMAT_R8G8B8A8_UINT";
    case VK_FORMAT_R8G8B8A8_SINT:
        return "VK_FORMAT_R8G8B8A8_SINT";
    case VK_FORMAT_R8G8B8A8_SRGB:
        return "VK_FORMAT_R8G8B8A8_SRGB";
    case VK_FORMAT_B8G8R8A8_UNORM:
        return "VK_FORMAT_B8G8R8A8_UNORM";
    case VK_FORMAT_B8G8R8A8_SNORM:
        return "VK_FORMAT_B8G8R8A8_SNORM";
    case VK_FORMAT_B8G8R8A8_USCALED:
        return "VK_FORMAT_B8G8R8A8_USCALED";
    case VK_FORMAT_B8G8R8A8_SSCALED:
        return "VK_FORMAT_B8G8R8A8_SSCALED";
    case VK_FORMAT_B8G8R8A8_UINT:
        return "VK_FORMAT_B8G8R8A8_UINT";
    case VK_FORMAT_B8G8R8A8_SINT:
        return "VK_FORMAT_B8G8R8A8_SINT";
    case VK_FORMAT_B8G8R8A8_SRGB:
        return "VK_FORMAT_B8G8R8A8_SRGB";
    case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
        return "VK_FORMAT_A8B8G8R8_UNORM_PACK32";
    case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
        return "VK_FORMAT_A8B8G8R8_SNORM_PACK32";
    case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
        return "VK_FORMAT_A8B8G8R8_USCALED_PACK32";
    case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
        return "VK_FORMAT_A8B8G8R8_SSCALED_PACK32";
    case VK_FORMAT_A8B8G8R8_UINT_PACK32:
        return "VK_FORMAT_A8B8G8R8_UINT_PACK32";
    case VK_FORMAT_A8B8G8R8_SINT_PACK32:
        return "VK_FORMAT_A8B8G8R8_SINT_PACK32";
    case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
        return "VK_FORMAT_A8B8G8R8_SRGB_PACK32";
    case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
        return "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
    case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
        return "VK_FORMAT_A2R10G10B10_SNORM_PACK32";
    case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
        return "VK_FORMAT_A2R10G10B10_USCALED_PACK32";
    case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
        return "VK_FORMAT_A2R10G10B10_SSCALED_PACK32";
    case VK_FORMAT_A2R10G10B10_UINT_PACK32:
        return "VK_FORMAT_A2R10G10B10_UINT_PACK32";
    case VK_FORMAT_A2R10G10B10_SINT_PACK32:
        return "VK_FORMAT_A2R10G10B10_SINT_PACK32";
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
        return "VK_FORMAT_A2B10G10R10_UNORM_PACK32";
    case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
        return "VK_FORMAT_A2B10G10R10_SNORM_PACK32";
    case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
        return "VK_FORMAT_A2B10G10R10_USCALED_PACK32";
    case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
        return "VK_FORMAT_A2B10G10R10_SSCALED_PACK32";
    case VK_FORMAT_A2B10G10R10_UINT_PACK32:
        return "VK_FORMAT_A2B10G10R10_UINT_PACK32";
    case VK_FORMAT_A2B10G10R10_SINT_PACK32:
        return "VK_FORMAT_A2B10G10R10_SINT_PACK32";
    case VK_FORMAT_R16_UNORM:
        return "VK_FORMAT_R16_UNORM";
    case VK_FORMAT_R16_SNORM:
        return "VK_FORMAT_R16_SNORM";
    case VK_FORMAT_R16_USCALED:
        return "VK_FORMAT_R16_USCALED";
    case VK_FORMAT_R16_SSCALED:
        return "VK_FORMAT_R16_SSCALED";
    case VK_FORMAT_R16_UINT:
        return "VK_FORMAT_R16_UINT";
    case VK_FORMAT_R16_SINT:
        return "VK_FORMAT_R16_SINT";
    case VK_FORMAT_R16_SFLOAT:
        return "VK_FORMAT_R16_SFLOAT";
    case VK_FORMAT_R16G16_UNORM:
        return "VK_FORMAT_R16G16_UNORM";
    case VK_FORMAT_R16G16_SNORM:
        return "VK_FORMAT_R16G16_SNORM";
    case VK_FORMAT_R16G16_USCALED:
        return "VK_FORMAT_R16G16_USCALED";
    case VK_FORMAT_R16G16_SSCALED:
        return "VK_FORMAT_R16G16_SSCALED";
    case VK_FORMAT_R16G16_UINT:
        return "VK_FORMAT_R16G16_UINT";
    case VK_FORMAT_R16G16_SINT:
        return "VK_FORMAT_R16G16_SINT";
    case VK_FORMAT_R16G16_SFLOAT:
        return "VK_FORMAT_R16G16_SFLOAT";
    case VK_FORMAT_R16G16B16_UNORM:
        return "VK_FORMAT_R16G16B16_UNORM";
    case VK_FORMAT_R16G16B16_SNORM:
        return "VK_FORMAT_R16G16B16_SNORM";
    case VK_FORMAT_R16G16B16_USCALED:
        return "VK_FORMAT_R16G16B16_USCALED";
    case VK_FORMAT_R16G16B16_SSCALED:
        return "VK_FORMAT_R16G16B16_SSCALED";
    case VK_FORMAT_R16G16B16_UINT:
        return "VK_FORMAT_R16G16B16_UINT";
    case VK_FORMAT_R16G16B16_SINT:
        return "VK_FORMAT_R16G16B16_SINT";
    case VK_FORMAT_R16G16B16_SFLOAT:
        return "VK_FORMAT_R16G16B16_SFLOAT";
    case VK_FORMAT_R16G16B16A16_UNORM:
        return "VK_FORMAT_R16G16B16A16_UNORM";
    case VK_FORMAT_R16G16B16A16_SNORM:
        return "VK_FORMAT_R16G16B16A16_SNORM";
    case VK_FORMAT_R16G16B16A16_USCALED:
        return "VK_FORMAT_R16G16B16A16_USCALED";
    case VK_FORMAT_R16G16B16A16_SSCALED:
        return "VK_FORMAT_R16G16B16A16_SSCALED";
    case VK_FORMAT_R16G16B16A16_UINT:
        return "VK_FORMAT_R16G16B16A16_UINT";
    case VK_FORMAT_R16G16B16A16_SINT:
        return "VK_FORMAT_R16G16B16A16_SINT";
    case VK_FORMAT_R16G16B16A16_SFLOAT:
        return "VK_FORMAT_R16G16B16A16_SFLOAT";
    case VK_FORMAT_R32_UINT:
        return "VK_FORMAT_R32_UINT";
    case VK_FORMAT_R32_SINT:
        return "VK_FORMAT_R32_SINT";
    case VK_FORMAT_R32_SFLOAT:
        return "VK_FORMAT_R32_SFLOAT";
    case VK_FORMAT_R32G32_UINT:
        return "VK_FORMAT_R32G32_UINT";
    case VK_FORMAT_R32G32_SINT:
        return "VK_FORMAT_R32G32_SINT";
    case VK_FORMAT_R32G32_SFLOAT:
        return "VK_FORMAT_R32G32_SFLOAT";
    case VK_FORMAT_R32G32B32_UINT:
        return "VK_FORMAT_R32G32B32_UINT";
    case VK_FORMAT_R32G32B32_SINT:
        return "VK_FORMAT_R32G32B32_SINT";
    case VK_FORMAT_R32G32B32_SFLOAT:
        return "VK_FORMAT_R32G32B32_SFLOAT";
    case VK_FORMAT_R32G32B32A32_UINT:
        return "VK_FORMAT_R32G32B32A32_UINT";
    case VK_FORMAT_R32G32B32A32_SINT:
        return "VK_FORMAT_R32G32B32A32_SINT";
    case VK_FORMAT_R32G32B32A32_SFLOAT:
        return "VK_FORMAT_R32G32B32A32_SFLOAT";
    case VK_FORMAT_R64_UINT:
        return "VK_FORMAT_R64_UINT";
    case VK_FORMAT_R64_SINT:
        return "VK_FORMAT_R64_SINT";
    case VK_FORMAT_R64_SFLOAT:
        return "VK_FORMAT_R64_SFLOAT";
    case VK_FORMAT_R64G64_UINT:
        return "VK_FORMAT_R64G64_UINT";
    case VK_FORMAT_R64G64_SINT:
        return "VK_FORMAT_R64G64_SINT";
    case VK_FORMAT_R64G64_SFLOAT:
        return "VK_FORMAT_R64G64_SFLOAT";
    case VK_FORMAT_R64G64B64_UINT:
        return "VK_FORMAT_R64G64B64_UINT";
    case VK_FORMAT_R64G64B64_SINT:
        return "VK_FORMAT_R64G64B64_SINT";
    case VK_FORMAT_R64G64B64_SFLOAT:
        return "VK_FORMAT_R64G64B64_SFLOAT";
    case VK_FORMAT_R64G64B64A64_UINT:
        return "VK_FORMAT_R64G64B64A64_UINT";
    case VK_FORMAT_R64G64B64A64_SINT:
        return "VK_FORMAT_R64G64B64A64_SINT";
    case VK_FORMAT_R64G64B64A64_SFLOAT:
        return "VK_FORMAT_R64G64B64A64_SFLOAT";
    default:
        return "Invalid VkFormat";
    }
}

StringView VkColorSpaceToString(VkColorSpaceKHR colorSpace)
{
    switch (colorSpace) {
    case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
        return "VK_COLOR_SPACE_SRGB_NONLINEAR_KHR";
    case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT:
        return "VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT";
    case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT:
        return "VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT";
    case VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT:
        return "VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT";
    case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT:
        return "VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT";
    case VK_COLOR_SPACE_BT709_LINEAR_EXT:
        return "VK_COLOR_SPACE_BT709_LINEAR_EXT";
    case VK_COLOR_SPACE_BT709_NONLINEAR_EXT:
        return "VK_COLOR_SPACE_BT709_NONLINEAR_EXT";
    case VK_COLOR_SPACE_BT2020_LINEAR_EXT:
        return "VK_COLOR_SPACE_BT2020_LINEAR_EXT";
    case VK_COLOR_SPACE_HDR10_ST2084_EXT:
        return "VK_COLOR_SPACE_HDR10_ST2084_EXT";
    case VK_COLOR_SPACE_DOLBYVISION_EXT:
        return "VK_COLOR_SPACE_DOLBYVISION_EXT";
    case VK_COLOR_SPACE_HDR10_HLG_EXT:
        return "VK_COLOR_SPACE_HDR10_HLG_EXT";
    case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT:
        return "VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT";
    case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT:
        return "VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT";
    case VK_COLOR_SPACE_PASS_THROUGH_EXT:
        return "VK_COLOR_SPACE_PASS_THROUGH_EXT";
    case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT:
        return "VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT";
    case VK_COLOR_SPACE_DISPLAY_NATIVE_AMD:
        return "VK_COLOR_SPACE_DISPLAY_NATIVE_AMD";
    default:
        return "Invalid VkColorSpaceKHR";
    }
}

StringView VkPresentModeToString(VkPresentModeKHR presentMode)
{
    switch (presentMode) {
    case VK_PRESENT_MODE_IMMEDIATE_KHR:
        return "VK_PRESENT_MODE_IMMEDIATE_KHR";
    case VK_PRESENT_MODE_MAILBOX_KHR:
        return "VK_PRESENT_MODE_MAILBOX_KHR";
    case VK_PRESENT_MODE_FIFO_KHR:
        return "VK_PRESENT_MODE_FIFO_KHR";
    case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
        return "VK_PRESENT_MODE_FIFO_RELAXED_KHR";
    case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
        return "VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR";
    case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
        return "VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR";
    default:
        return "Invalid VkColorSpaceKHR";
    }
}

} // namespace dusk