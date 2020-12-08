#include <Dusk/Vulkan/VulkanTexture.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk {

DUSK_VULKAN_API
VulkanTexture::~VulkanTexture()
{

}

DUSK_VULKAN_API
bool VulkanTexture::Load(const TextureData * data, Options opts /*= Options()*/)
{
    DuskBenchmarkStart();

    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    const auto& size = data->GetSize();
    // VkDeviceSize imageSize = size.x * size.y * data->GetComponents();

    VkImageCreateInfo imageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = GetVkDataFormat(data->GetComponents(), data->GetDataType()),
        .extent = {
            .width = size.x,
            .height = size.y,
            .depth = 1,
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    if (vkCreateImage(gfx->GetDevice(), &imageCreateInfo, nullptr, &_vkImage) != VK_SUCCESS) {
        DuskLogError("Failed to create image");
        return false;
    }

    VkDeviceMemory memory;

    // TODO:

    DuskBenchmarkEnd("VulkanTexture::Load");
    return true;
}

DUSK_VULKAN_API
VkFormat VulkanTexture::GetVkDataFormat(int components, const TextureData::DataType& type)
{
    if (components == 1) {
        switch(type) {
            case TextureData::DataType::UnsignedByte:
                return VK_FORMAT_R8_UINT;
            case TextureData::DataType::Byte:
                return VK_FORMAT_R8_SINT;
            case TextureData::DataType::UnsignedShort:
                return VK_FORMAT_R16_UINT;
            case TextureData::DataType::Short:
                return VK_FORMAT_R16_SINT;
            case TextureData::DataType::UnsignedInt:
                return VK_FORMAT_R32_UINT;
            case TextureData::DataType::Int:
                return VK_FORMAT_R32_SINT;
        }
    }
    else if (components == 2) {
        switch(type) {
            case TextureData::DataType::UnsignedByte:
                return VK_FORMAT_R8G8_UINT;
            case TextureData::DataType::Byte:
                return VK_FORMAT_R8G8_SINT;
            case TextureData::DataType::UnsignedShort:
                return VK_FORMAT_R16G16_UINT;
            case TextureData::DataType::Short:
                return VK_FORMAT_R16G16_SINT;
            case TextureData::DataType::UnsignedInt:
                return VK_FORMAT_R32G32_UINT;
            case TextureData::DataType::Int:
                return VK_FORMAT_R32G32_SINT;
        }
    }
    else if (components == 3) {
        switch(type) {
            case TextureData::DataType::UnsignedByte:
                return VK_FORMAT_R8G8B8_UINT;
            case TextureData::DataType::Byte:
                return VK_FORMAT_R8G8B8_SINT;
            case TextureData::DataType::UnsignedShort:
                return VK_FORMAT_R16G16B16_UINT;
            case TextureData::DataType::Short:
                return VK_FORMAT_R16G16B16_SINT;
            case TextureData::DataType::UnsignedInt:
                return VK_FORMAT_R32G32B32_UINT;
            case TextureData::DataType::Int:
                return VK_FORMAT_R32G32B32_SINT;
        }
    }
    else if (components == 4) {
        switch(type) {
            case TextureData::DataType::UnsignedByte:
                return VK_FORMAT_R8G8B8A8_UINT;
            case TextureData::DataType::Byte:
                return VK_FORMAT_R8G8B8A8_SINT;
            case TextureData::DataType::UnsignedShort:
                return VK_FORMAT_R16G16B16A16_UINT;
            case TextureData::DataType::Short:
                return VK_FORMAT_R16G16B16A16_SINT;
            case TextureData::DataType::UnsignedInt:
                return VK_FORMAT_R32G32B32A32_UINT;
            case TextureData::DataType::Int:
                return VK_FORMAT_R32G32B32A32_SINT;
        }
    }

    return VK_FORMAT_UNDEFINED;
}

} // namespace Dusk
