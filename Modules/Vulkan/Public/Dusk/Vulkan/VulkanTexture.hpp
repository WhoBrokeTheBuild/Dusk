#ifndef DUSK_VULKAN_TEXTURE_HPP
#define DUSK_VULKAN_TEXTURE_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>

#include <Dusk/Texture.hpp>

namespace Dusk::Vulkan {

#define DUSK_VULKAN_TEXTURE(x) (dynamic_cast<Dusk::Vulkan::VulkanTexture *>(x))

class DUSK_VULKAN_API VulkanTexture : public Texture
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanTexture)

    VulkanTexture() = default;

    virtual ~VulkanTexture();

    bool Load(const std::unique_ptr<TextureData>& data, Options opts = Options()) override;

    VkImage GetVkImage() const;

private:

    VkImage _vkImage;

}; // class VulkanTexture

inline VkFormat GetVkDataFormat(int components, const TextureDataType& dataType)
{
    if (components == 1) {
        switch(dataType) {
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
    }
    else if (components == 2) {
        switch(dataType) {
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
    }
    else if (components == 3) {
        switch(dataType) {
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
    }
    else if (components == 4) {
        switch(dataType) {
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
    }

    return VK_FORMAT_UNDEFINED;
}

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_TEXTURE_HPP