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
bool Texture::Load(const TextureData& data)
{
    DuskBenchmarkStart();

    VkDeviceSize imageSize = data.Size.x * data.Size.y * GetChannelCount(data.DataFormat);

    VkImage vkImage;
    VkDeviceMemory vkDeviceMemory;

    VkImageCreateInfo vkImageCreateInfo = {};
    vkImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    vkImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    vkImageCreateInfo.extent.width = static_cast<uint32_t>(data.Size.x);
    vkImageCreateInfo.extent.height = static_cast<uint32_t>(data.Size.y);
    vkImageCreateInfo.extent.depth = 1;
    vkImageCreateInfo.mipLevels = 1;
    vkImageCreateInfo.arrayLayers = 1;
    vkImageCreateInfo.format = GetVkDataFormat(data.DataFormat, data.DataType);
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
int Texture::GetChannelCount(const TextureDataFormat& format)
{
    switch (format) {
    case TextureDataFormat::R:
        return 1;
    case TextureDataFormat::RG:
        return 2;
    case TextureDataFormat::RGB:
        return 3;
    case TextureDataFormat::RGBA:
        return 4;
    }

    return -1;
}

DUSK_VULKAN_API
VkFormat Texture::GetVkDataFormat(const TextureDataFormat& format, const TextureDataType& type)
{
    switch (format) {
    case TextureDataFormat::R:
        switch(type) {
            case TextureDataType::UnsignedByte:
                return VK_FORMAT_R8_UINT;
            case TextureDataType::Byte:
                return VK_FORMAT_R8_SINT;
            case TextureDataType::UnsignedShort:
                return VK_FORMAT_R16_UINT;
            case TextureDataType::Short:
                return VK_FORMAT_R16_SINT;
            case TextureDataType::UnsignedInt:
                return VK_FORMAT_R32_UINT;
            case TextureDataType::Int:
                return VK_FORMAT_R32_SINT;
        }
        break;
    case TextureDataFormat::RG:
        switch(type) {
            case TextureDataType::UnsignedByte:
                return VK_FORMAT_R8G8_UINT;
            case TextureDataType::Byte:
                return VK_FORMAT_R8G8_SINT;
            case TextureDataType::UnsignedShort:
                return VK_FORMAT_R16G16_UINT;
            case TextureDataType::Short:
                return VK_FORMAT_R16G16_SINT;
            case TextureDataType::UnsignedInt:
                return VK_FORMAT_R32G32_UINT;
            case TextureDataType::Int:
                return VK_FORMAT_R32G32_SINT;
        }
        break;
    case TextureDataFormat::RGB:
        switch(type) {
            case TextureDataType::UnsignedByte:
                return VK_FORMAT_R8G8B8_UINT;
            case TextureDataType::Byte:
                return VK_FORMAT_R8G8B8_SINT;
            case TextureDataType::UnsignedShort:
                return VK_FORMAT_R16G16B16_UINT;
            case TextureDataType::Short:
                return VK_FORMAT_R16G16B16_SINT;
            case TextureDataType::UnsignedInt:
                return VK_FORMAT_R32G32B32_UINT;
            case TextureDataType::Int:
                return VK_FORMAT_R32G32B32_SINT;
        }
        break;
    case TextureDataFormat::RGBA:
        switch(type) {
            case TextureDataType::UnsignedByte:
                return VK_FORMAT_R8G8B8A8_UINT;
            case TextureDataType::Byte:
                return VK_FORMAT_R8G8B8A8_SINT;
            case TextureDataType::UnsignedShort:
                return VK_FORMAT_R16G16B16A16_UINT;
            case TextureDataType::Short:
                return VK_FORMAT_R16G16B16A16_SINT;
            case TextureDataType::UnsignedInt:
                return VK_FORMAT_R32G32B32A32_UINT;
            case TextureDataType::Int:
                return VK_FORMAT_R32G32B32A32_SINT;
        }
        break;
    }

    return VK_FORMAT_UNDEFINED;
}

} // namespace Dusk::Vulkan
