#ifndef DUSK_VULKAN_TEXTURE_HPP
#define DUSK_VULKAN_TEXTURE_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Asset.hpp>
#include <Dusk/Path.hpp>
#include <Dusk/Span.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>

#include <memory>

namespace dusk {

class DUSK_API VulkanTexture : Asset
{
public:

    using Pointer = std::shared_ptr<VulkanTexture>;

    VulkanTexture() = default;
    
    virtual ~VulkanTexture();

    bool LoadFromFile(const Path& path, VkSamplerCreateInfo samplerCreateInfo = {});

    bool LoadFromBuffer(const uint8_t * buffer, size_t size, VkSamplerCreateInfo samplerCreateInfo = {});

    inline bool LoadFromBuffer(Span<const uint8_t> buffer, VkSamplerCreateInfo samplerCreateInfo = {}) {
        return LoadFromBuffer(buffer.data(), buffer.size(), samplerCreateInfo);
    }

    bool LoadFromPixels(const uint8_t * pixels, size_t width, size_t height, size_t components, VkSamplerCreateInfo samplerCreateInfo = {});

    void Destroy();

    bool CanReload() const override {
        return not _path.empty();
    }

    bool Reload() override;

    bool Serialize(JSON::Object& data) const override;

    bool Deserialize(const JSON::Object& data) override;

    inline VkImageView GetImageView() const {
        return _imageView;
    }

    inline VkSampler GetSampler() const {
        return _sampler;
    }

private:

    void createImage(const uint8_t * pixels, size_t size);

    Path _path;

    VkExtent2D _extent;

    VkSamplerCreateInfo _samplerCreateInfo;

    VkImage _image = VK_NULL_HANDLE;

    VmaAllocation _allocation = VK_NULL_HANDLE;

    VkImageView _imageView = VK_NULL_HANDLE;

    VkSampler _sampler = VK_NULL_HANDLE;

}; // class VulkanTexture

} // namespace dusk

#endif // DUSK_VULKAN_TEXTURE_HPP