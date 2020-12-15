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

    VkFormat GetVkDataFormat(int components, const TextureData::DataType& type);

    VkImage _vkImage;

}; // class VulkanTexture

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_TEXTURE_HPP