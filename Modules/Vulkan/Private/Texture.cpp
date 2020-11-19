#include <Dusk/Vulkan/Texture.hpp>
#include <Dusk/Benchmark.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
Texture::Texture(VkDevice vkDevice)
    : _vkDevice(vkDevice)
{ }

DUSK_VULKAN_API
Texture::~Texture()
{

}

DUSK_VULKAN_API
bool Texture::Load(const TextureData * data, Options opts /*= Options()*/)
{
    DuskBenchmarkStart();

    const auto& size = data->GetSize();
    // VkDeviceSize imageSize = size.x * size.y * data->GetComponents();

    VkImage vkImage;
    // VkDeviceMemory vkDeviceMemory;

    VkImageCreateInfo vkImageCreateInfo = {};
    vkImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    vkImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    vkImageCreateInfo.extent.width = size.x;
    vkImageCreateInfo.extent.height = size.y;
    vkImageCreateInfo.extent.depth = 1;
    vkImageCreateInfo.mipLevels = 1;
    vkImageCreateInfo.arrayLayers = 1;
    vkImageCreateInfo.format = GetVkDataFormat(data->GetComponents(), data->GetDataType());
    vkImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    vkImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    vkImageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    vkImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    vkImageCreateInfo.flags = 0;

    if (vkCreateImage(_vkDevice, &vkImageCreateInfo, nullptr, &vkImage) != VK_SUCCESS) {
        return false;
    }

    DuskBenchmarkEnd("Vulkan::Texture::Load");
    return true;
}

DUSK_VULKAN_API
void Texture::Bind() 
{
}

DUSK_VULKAN_API
VkFormat Texture::GetVkDataFormat(int components, const TextureData::DataType& type)
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

} // namespace Dusk::Vulkan
