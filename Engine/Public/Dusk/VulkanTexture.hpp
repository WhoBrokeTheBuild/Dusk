#ifndef DUSK_VULKAN_TEXTURE_HPP
#define DUSK_VULKAN_TEXTURE_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Path.hpp>
#include <Dusk/Span.hpp>
#include <Dusk/NonCopyable.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>

#include <memory>

namespace dusk {

class DUSK_API VulkanTexture : NonCopyable
{
public:

    using Pointer = std::shared_ptr<VulkanTexture>;

    VulkanTexture() = default;
    
    virtual ~VulkanTexture();

    bool LoadFromFile(const Path& path, vk::SamplerCreateInfo samplerCreateInfo = {});

    bool LoadFromBuffer(const uint8_t * buffer,size_t size, vk::SamplerCreateInfo samplerCreateInfo = {});

    inline bool LoadFromBuffer(Span<const uint8_t> buffer, vk::SamplerCreateInfo samplerCreateInfo = {}) {
        return LoadFromBuffer(buffer.data(), buffer.size(), samplerCreateInfo);
    }

    void Destroy();

    bool Reload();

    inline bool CanReload() const {
        return not _path.empty();
    }

private:

    void createImage(uint8_t * pixels, size_t size);

    Path _path;

    vk::Extent2D _extent;

    vk::SamplerCreateInfo _samplerCreateInfo;

    vk::Image _image = VK_NULL_HANDLE;

    VmaAllocation _allocation = VK_NULL_HANDLE;

    vk::ImageView _imageView = VK_NULL_HANDLE;

    vk::Sampler _sampler = VK_NULL_HANDLE;

}; // class VulkanTexture

} // namespace dusk

#endif // DUSK_VULKAN_TEXTURE_HPP