#include <Dusk/Vulkan/VulkanMesh.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk {

DUSK_VULKAN_API
VulkanMesh::~VulkanMesh()
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    for (VkBuffer buffer : _buffers) {
        vkDestroyBuffer(gfx->GetDevice(), buffer, nullptr);
    }

    for (VkDeviceMemory memory : _bufferMemories) {
        vkFreeMemory(gfx->GetDevice(), memory, nullptr);
    }
}

DUSK_VULKAN_API
void VulkanMesh::Render()
{

}

DUSK_VULKAN_API
bool VulkanMesh::Load(const MeshData * data)
{
    bool result;

    if (!data->GetIndices().empty()) {
        DuskLogError("Indexed drawing with vulkan ???");
        return false;
    }

    result = AddBuffer(
        reinterpret_cast<const void *>(data->GetVertices().data()), 
        sizeof(float) * data->GetVertices().size(), 
        sizeof(float) * 4, 
        VK_FORMAT_R32G32B32A32_SFLOAT,
        VK_VERTEX_INPUT_RATE_VERTEX,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    if (result) {
        DuskLogError("Failed to create vertex buffer");
        return false;
    }

    if (!data->GetNormals().empty()) {
        result = AddBuffer(
            reinterpret_cast<const void *>(data->GetNormals().data()), 
            sizeof(float) * data->GetNormals().size(), 
            sizeof(float) * 4, 
            VK_FORMAT_R32G32B32A32_SFLOAT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (result) {
            DuskLogError("Failed to create normal buffer");
            return false;
        }
    }

    if (!data->GetUVs().empty()) {
        result = AddBuffer(
            reinterpret_cast<const void *>(data->GetUVs().data()), 
            sizeof(float) * data->GetUVs().size(), 
            sizeof(float) * 2, 
            VK_FORMAT_R32G32_SFLOAT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (result) {
            DuskLogError("Failed to create UV buffer");
            return false;
        }
    }

    if (!data->GetColors().empty()) {
        result = AddBuffer(
            reinterpret_cast<const void *>(data->GetColors().data()), 
            sizeof(float) * data->GetColors().size(), 
            sizeof(float) * 4, 
            VK_FORMAT_R32G32B32A32_SFLOAT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (result) {
            DuskLogError("Failed to create color buffer");
            return false;
        }
    }

    if (!data->GetTangents().empty()) {
        result = AddBuffer(
            reinterpret_cast<const void *>(data->GetTangents().data()), 
            sizeof(float) * data->GetTangents().size(), 
            sizeof(float) * 4, 
            VK_FORMAT_R32G32B32A32_SFLOAT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (result) {
            DuskLogError("Failed to create tangent buffer");
            return false;
        }
    }

    if (!data->GetBitangents().empty()) {
        result = AddBuffer(
            reinterpret_cast<const void *>(data->GetBitangents().data()), 
            sizeof(float) * data->GetBitangents().size(), 
            sizeof(float) * 4, 
            VK_FORMAT_R32G32B32A32_SFLOAT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (result) {
            DuskLogError("Failed to create bitangent buffer");
            return false;
        }
    }

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = static_cast<uint32_t>(_bindings.size()),
        .pVertexBindingDescriptions = _bindings.data(),
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(_attributes.size()),
        .pVertexAttributeDescriptions = _attributes.data(),
    };

    return true;
}

bool VulkanMesh::AddBuffer(const void * data, VkDeviceSize size, uint32_t stride, 
    VkFormat format, VkVertexInputRate inputRate, VkBufferUsageFlags usage)
{
    VkResult result;

    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    _bindings.push_back({
        .binding = _nextBinding,
        .stride = stride,
        .inputRate = inputRate,
    });

    _attributes.push_back({
        .location = _nextLocation,
        .binding = _nextBinding,
        .format = format,
        .offset = 0,
    });

    VkBuffer buffer;
    VkDeviceMemory memory;

    if (!gfx->CreateBuffer(buffer, memory, size, usage, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
        return false;
    }

    void * ptr;
    vkMapMemory(gfx->GetDevice(), memory, 0, size, 0, &ptr);
    memcpy(ptr, data, static_cast<size_t>(size));
    vkUnmapMemory(gfx->GetDevice(), memory);

    _buffers.push_back(buffer);
    _bufferMemories.push_back(memory);

    ++_nextBinding;
    ++_nextLocation;

    return true;
}

} // namespace Dusk