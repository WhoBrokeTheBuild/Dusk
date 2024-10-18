#include <Dusk/VulkanTexture.hpp>
#include <Dusk/Graphics.hpp>

#include <print>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace dusk {

DUSK_API
VulkanTexture::~VulkanTexture()
{
    Destroy();
}

DUSK_API
bool VulkanTexture::LoadFromFile(
    const Path& path,
    vk::SamplerCreateInfo samplerCreateInfo /*= {}*/
) {
    Destroy();

    int width;
    int height;
    int channelsInFile;
    uint8_t * pixels = nullptr;

    auto assetPathList = GetAssetPathList();

    if (path.is_absolute()) {
        pixels = stbi_load(path.c_str(), &width, &height, &channelsInFile, STBI_rgb_alpha);
        if (pixels) {
            _path = path;
        }
    }
    else {
        for (const auto& assetPath : assetPathList) {
            auto fullPath = assetPath / path;
            pixels = stbi_load(path.c_str(), &width, &height, &channelsInFile, STBI_rgb_alpha);
            if (pixels) {
                _path = fullPath;
                break;
            }
        }
    }

    if (_path.empty()) {
        return false;
    }

    _extent.setWidth(width);
    _extent.setHeight(height);
    _samplerCreateInfo = samplerCreateInfo;
    
    size_t pixelsSize = width * height * STBI_rgb_alpha;
    createImage(pixels, pixelsSize);

    stbi_image_free(pixels);

    // TODO: setLoaded
    return true;
}

DUSK_API
bool VulkanTexture::LoadFromBuffer(
    const uint8_t * buffer,
    size_t size,
    vk::SamplerCreateInfo samplerCreateInfo /*= {}*/
) {
    Destroy();

    int width;
    int height;
    uint8_t * pixels = stbi_load_from_memory(buffer, size, &width, &height, nullptr, STBI_rgb_alpha);
    if (not pixels) {
        return false;
    }

    _path.clear();
    _extent.setWidth(width);
    _extent.setHeight(height);
    _samplerCreateInfo = samplerCreateInfo;
    
    size_t pixelsSize = width * height * STBI_rgb_alpha;
    createImage(pixels, pixelsSize);

    stbi_image_free(pixels);

    // TODO: setLoaded
    return true;
}

DUSK_API
void VulkanTexture::Destroy()
{
    if (_sampler) {
        Graphics::Device.destroySampler(_sampler);
        _sampler = VK_NULL_HANDLE;
    }

    if (_imageView) {
        Graphics::Device.destroyImageView(_imageView);
        _imageView = VK_NULL_HANDLE;
    }

    if (_image) {
        Graphics::Device.destroyImage(_image);
        _image = VK_NULL_HANDLE;
    }

    if (_allocation) {
        vmaFreeMemory(Graphics::Allocator, _allocation);
        _allocation = VK_NULL_HANDLE;
    }
}

DUSK_API
bool VulkanTexture::Reload()
{
    return LoadFromFile(_path);
}

DUSK_API
void VulkanTexture::createImage(uint8_t * pixels, vk::DeviceSize size)
{
    auto stagingBufferCreateInfo = vk::BufferCreateInfo()
        .setSize(size)
        .setUsage(vk::BufferUsageFlagBits::eTransferSrc);

    auto stagingAllocationCreateInfo = VmaAllocationCreateInfo{
        .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_CPU_TO_GPU,
    };

    VmaAllocationInfo stagingAllocationInfo;
    auto[stagingBuffer, stagingAllocation] = Graphics::CreateBuffer(
        stagingBufferCreateInfo,
        stagingAllocationCreateInfo,
        &stagingAllocationInfo
    );

    memcpy(stagingAllocationInfo.pMappedData, pixels, size);

    auto imageCreateInfo = vk::ImageCreateInfo()
        .setImageType(vk::ImageType::e2D)
        .setFormat(vk::Format::eR8G8B8A8Srgb)
        .setTiling(vk::ImageTiling::eOptimal)
        .setExtent(vk::Extent3D(_extent, 1))
        .setMipLevels(1) // TODO: Investigate
        .setArrayLayers(1) // TODO: Investigate
        .setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
        .setInitialLayout(vk::ImageLayout::eUndefined)
        .setSharingMode(vk::SharingMode::eExclusive)
        .setSamples(vk::SampleCountFlagBits::e1); // TODO: Investigate

    auto allocationCreateInfo = VmaAllocationCreateInfo{
        .usage = VMA_MEMORY_USAGE_GPU_ONLY,
    };

    std::tie(_image, _allocation) = Graphics::CreateImage(
        imageCreateInfo,
        allocationCreateInfo
    );

    auto subresourceLayers = vk::ImageSubresourceLayers()
        .setAspectMask(vk::ImageAspectFlagBits::eColor)
        .setMipLevel(0)
        .setBaseArrayLayer(0)
        .setLayerCount(1);

    auto region = vk::BufferImageCopy()
        .setBufferOffset(0)
        .setBufferRowLength(0)
        .setBufferImageHeight(0)
        .setImageSubresource(subresourceLayers)
        .setImageOffset(vk::Offset3D(0, 0, 0))
        .setImageExtent(vk::Extent3D(_extent, 1));

    Graphics::CopyBufferToImage(stagingBuffer, _image, region);

    vmaDestroyBuffer(Graphics::Allocator, stagingBuffer, stagingAllocation);

    auto subresourceRange = vk::ImageSubresourceRange()
        .setAspectMask(vk::ImageAspectFlagBits::eColor)
        .setBaseMipLevel(0)
        .setLevelCount(1)
        .setBaseArrayLayer(0)
        .setLayerCount(1);

    auto imageViewCreateInfo = vk::ImageViewCreateInfo()
        .setImage(_image)
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(vk::Format::eR8G8B8A8Srgb)
        .setSubresourceRange(subresourceRange);

    _imageView = Graphics::Device.createImageView(imageViewCreateInfo);

    _sampler = Graphics::Device.createSampler(_samplerCreateInfo);
}

} // namespace dusk