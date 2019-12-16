#ifndef DUSK_VULKAN_TEXTURE_HPP
#define DUSK_VULKAN_TEXTURE_HPP

#include <Dusk/Vulkan/Config.hpp>
#include <Dusk/Graphics/Texture.hpp>

namespace Dusk::Vulkan {

class DUSK_VULKAN_API Texture : public ITexture
{
public:

    explicit Texture(VkDevice vkDevice);

    virtual ~Texture();

    bool Load(const ITextureData * data) override;

    void Bind() override;

private:

    VkFormat GetVkDataFormat(int components, const ITextureData::DataType& type);

    VkDevice _vkDevice;

}; // class Texture

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_TEXTURE_HPP