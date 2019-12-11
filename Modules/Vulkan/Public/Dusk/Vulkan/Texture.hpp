#ifndef DUSK_VULKAN_TEXTURE_HPP
#define DUSK_VULKAN_TEXTURE_HPP

#include <Dusk/Vulkan/Config.hpp>
#include <Dusk/Graphics/Texture.hpp>

namespace Dusk::Vulkan {

class DUSK_VULKAN_API Texture : public ITexture
{
public:

    explicit Texture(VkPhysicalDevice vkPhysicalDevice);

    virtual ~Texture();

    bool Load(const TextureData& data) override;

    void Bind() override;

private:

    VkPhysicalDevice _vkPhysicalDevice;

}; // class Texture

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_TEXTURE_HPP