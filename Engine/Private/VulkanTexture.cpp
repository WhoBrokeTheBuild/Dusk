#include <Dusk/VulkanTexture.hpp>
#include <Dusk/Graphics.hpp>
#include <Dusk/Log.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace dusk {

DUSK_API
VulkanTexture::~VulkanTexture()
{
    Destroy();
}

bool VulkanTexture::Serialize(JSON::Object& data) const
{
    if (not CanReload()) {
        return false;
    }

    data["path"] = FindShortestAssetPath(_path);
    data["magFilter"] = _samplerCreateInfo.magFilter;
    data["minFilter"] = _samplerCreateInfo.minFilter;
    data["mipmapMode"] = _samplerCreateInfo.mipmapMode;
    data["addressModeU"] = _samplerCreateInfo.addressModeU;
    data["addressModeV"] = _samplerCreateInfo.addressModeV;

    return true;
}

bool VulkanTexture::Deserialize(const JSON::Object& data)
{
    if (! data.contains("path")) {
        return false;
    }

    auto path = data["path"].get<std::string>();
    auto samplerCreateInfo = VkSamplerCreateInfo{
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = data.value<VkFilter>("magFilter", VK_FILTER_NEAREST),
        .minFilter = data.value<VkFilter>("minFilter", VK_FILTER_NEAREST),
        .mipmapMode = data.value<VkSamplerMipmapMode>("mipmapMode", VK_SAMPLER_MIPMAP_MODE_NEAREST),
        .addressModeU = data.value<VkSamplerAddressMode>("addressModeU", VK_SAMPLER_ADDRESS_MODE_REPEAT),
        .addressModeV = data.value<VkSamplerAddressMode>("addressModeV", VK_SAMPLER_ADDRESS_MODE_REPEAT),
    };

    return LoadFromFile(data["path"], samplerCreateInfo);
}

DUSK_API
bool VulkanTexture::LoadFromFile(
    const Path& path,
    VkSamplerCreateInfo samplerCreateInfo /*= {}*/
) {
    Destroy();

    int width;
    int height;
    int channelsInFile;
    uint8_t * pixels = nullptr;

    Log(DUSK_ANCHOR, "Loading {}", path.string());

    auto assetPathList = GetAssetPathList();

    if (path.is_absolute()) {
        pixels = stbi_load(path.string().c_str(), &width, &height, &channelsInFile, STBI_rgb_alpha);
        if (pixels) {
            _path = path;
        }
    }
    else {
        for (const auto& assetPath : assetPathList) {
            auto fullPath = assetPath / path;
            Log(DUSK_ANCHOR, "Checking {}", fullPath.string());
            pixels = stbi_load(fullPath.string().c_str(), &width, &height, &channelsInFile, STBI_rgb_alpha);
            if (pixels) {
                _path = fullPath;
                break;
            }
        }
    }

    if (_path.empty()) {
        return false;
    }

    Log(DUSK_ANCHOR, "Loaded {}", path.string());

    _extent.width = width;
    _extent.height = height;
    _samplerCreateInfo = samplerCreateInfo;
    
    size_t pixelsSize = width * height * STBI_rgb_alpha;
    createImage(pixels, pixelsSize);

    stbi_image_free(pixels);

    SetLoaded(true);
    return true;
}

DUSK_API
bool VulkanTexture::LoadFromBuffer(
    const uint8_t * buffer,
    size_t size,
    VkSamplerCreateInfo samplerCreateInfo /*= {}*/
) {
    Destroy();

    int width;
    int height;
    uint8_t * pixels = stbi_load_from_memory(buffer, size, &width, &height, nullptr, STBI_rgb_alpha);
    if (not pixels) {
        return false;
    }

    _path.clear();
    _extent.width = width;
    _extent.height = height;
    _samplerCreateInfo = samplerCreateInfo;
    
    size_t pixelsSize = width * height * STBI_rgb_alpha;
    createImage(pixels, pixelsSize);

    stbi_image_free(pixels);

    SetLoaded(true);
    return true;
}

DUSK_API
bool VulkanTexture::LoadFromPixels(
    const uint8_t * pixels,
    size_t width,
    size_t height,
    size_t components,
    VkSamplerCreateInfo samplerCreateInfo /*= {}*/
) {
    // TODO:
    assert(components == 4);

    _path.clear();
    _extent.width = width;
    _extent.height = height;
    _samplerCreateInfo = samplerCreateInfo;

    createImage(pixels, width * height * components);

    SetLoaded(true);
    return true;
}

DUSK_API
void VulkanTexture::Destroy()
{
    if (_sampler) {
        vkDestroySampler(Graphics::Device, _sampler, nullptr);
        _sampler = VK_NULL_HANDLE;
    }

    if (_imageView) {
        vkDestroyImageView(Graphics::Device, _imageView, nullptr);
        _imageView = VK_NULL_HANDLE;
    }

    if (_image) {
        vmaDestroyImage(Graphics::Allocator, _image, _allocation);
        _image = VK_NULL_HANDLE;
        _allocation = VK_NULL_HANDLE;
    }

    SetLoaded(false);
}

DUSK_API
bool VulkanTexture::Reload()
{
    return LoadFromFile(_path);
}

DUSK_API
void VulkanTexture::createImage(const uint8_t * pixels, VkDeviceSize size)
{
    VkResult result;

    auto stagingBufferCreateInfo = VkBufferCreateInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    };

    auto stagingAllocationCreateInfo = VmaAllocationCreateInfo{
        .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_CPU_TO_GPU,
    };

    VkBuffer stagingBuffer;
    VmaAllocation stagingAllocation;
    VmaAllocationInfo stagingAllocationInfo;

    result = vmaCreateBuffer(
        Graphics::Allocator,
        &stagingBufferCreateInfo,
        &stagingAllocationCreateInfo,
        &stagingBuffer,
        &stagingAllocation,
        &stagingAllocationInfo
    );
    CheckVkResult("vmaCreateBuffer", result);

    memcpy(stagingAllocationInfo.pMappedData, pixels, size);

    auto imageCreateInfo = VkImageCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_SRGB,
        .extent = VkExtent3D{
            .width = _extent.width,
            .height = _extent.height,
            .depth = 1
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    auto allocationCreateInfo = VmaAllocationCreateInfo{
        .usage = VMA_MEMORY_USAGE_GPU_ONLY,
    };

    result = vmaCreateImage(
        Graphics::Allocator,
        &imageCreateInfo,
        &allocationCreateInfo,
        &_image,
        &_allocation,
        nullptr
    );
    CheckVkResult("vmaCreateImage", result);

    auto subresourceLayers = VkImageSubresourceLayers{
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .mipLevel = 0,
        .baseArrayLayer = 0,
        .layerCount = 1,
    };

    auto region = VkBufferImageCopy{
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = subresourceLayers,
        .imageOffset = VkOffset3D{0, 0, 0},
        .imageExtent = VkExtent3D{
            .width = _extent.width,
            .height = _extent.height,
            .depth = 1,
        },
    };

    Graphics::CopyBufferToImage(
        stagingBuffer,
        _image,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        region
    );

    vmaDestroyBuffer(Graphics::Allocator, stagingBuffer, stagingAllocation);

    auto subresourceRange = VkImageSubresourceRange{
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
    };

    auto imageViewCreateInfo = VkImageViewCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = _image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_SRGB,
        .subresourceRange = subresourceRange,
    };

    result = vkCreateImageView(Graphics::Device, &imageViewCreateInfo, nullptr, &_imageView);
    CheckVkResult("vkCreateImageView", result);

    _samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

    result = vkCreateSampler(Graphics::Device, &_samplerCreateInfo, nullptr, &_sampler);
    CheckVkResult("vkCreateSampler", result);
}

} // namespace dusk