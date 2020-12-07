#ifndef DUSK_VULKAN_TEXTURE_HPP
#define DUSK_VULKAN_TEXTURE_HPP

#include <Dusk/Vulkan/Config.hpp>
#include <Dusk/Graphics/Texture.hpp>

namespace Dusk {

#define DUSK_VULKAN_TEXTURE(x) (dynamic_cast<Dusk::VulkanTexture *>(x))

class DUSK_VULKAN_API VulkanTexture : public Texture
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanTexture)

    VulkanTexture() = default;

    virtual ~VulkanTexture();

    inline std::string GetClassID() const override {
        return "DuskTexture";
    }

    bool Load(const TextureData * data, Options opts = Options()) override;

    VkImage GetVkImage() const;

private:

    VkFormat GetVkDataFormat(int components, const TextureData::DataType& type);

    VkImage _vkImage;

}; // class VulkanTexture

} // namespace Dusk

#endif // DUSK_VULKAN_TEXTURE_HPP