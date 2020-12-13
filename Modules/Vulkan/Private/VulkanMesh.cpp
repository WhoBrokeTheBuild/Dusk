#include <Dusk/Vulkan/VulkanMesh.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
VulkanMesh::~VulkanMesh()
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    for (VkBuffer buffer : _vkBuffers) {
        vkDestroyBuffer(gfx->GetVkDevice(), buffer, nullptr);
    }

    for (VkDeviceMemory memory : _vkBufferMemories) {
        vkFreeMemory(gfx->GetVkDevice(), memory, nullptr);
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

    const auto& indices = data->GetIndices();
    const auto& vertices = data->GetVertices();
    const auto& normals = data->GetNormals();
    const auto& uvs = data->GetUVs();
    const auto& colors = data->GetColors();
    const auto& tangents = data->GetTangents();
    const auto& bitangents = data->GetBitangents();

    switch (data->GetMode()) {
    case MeshData::Mode::Points:
        _vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        break;
    case MeshData::Mode::Lines:
        _vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        break;
    case MeshData::Mode::LineStrip:
        _vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        break;
    case MeshData::Mode::Triangles:
        _vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        break;
    case MeshData::Mode::TriangleStrip:
        _vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        break;
    }

    if (indices.empty()) {
        _indexed = false;
        // We need to convert the number of floats, into the number of vec4s
        _count = vertices.size() / 4;
    }
    else {
        DuskLogFatal("UNTESTED");

        _indexed = true;
        _count = indices.size();

        result = AddBuffer(
            static_cast<const void *>(data->GetIndices().data()), 
            sizeof(unsigned) * data->GetIndices().size(), 
            sizeof(unsigned), 
            VK_FORMAT_R32_UINT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        if (!result) {
            DuskLogError("Failed to create index buffer");
            return false;
        }
    }

    result = AddBuffer(
        static_cast<const void *>(vertices.data()), 
        sizeof(float) * vertices.size(), 
        sizeof(float) * 4, 
        VK_FORMAT_R32G32B32A32_SFLOAT,
        VK_VERTEX_INPUT_RATE_VERTEX,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    if (!result) {
        DuskLogError("Failed to create vertex buffer");
        return false;
    }

    if (!normals.empty()) {
        result = AddBuffer(
            static_cast<const void *>(normals.data()), 
            sizeof(float) * normals.size(), 
            sizeof(float) * 4, 
            VK_FORMAT_R32G32B32A32_SFLOAT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (!result) {
            DuskLogError("Failed to create normal buffer");
            return false;
        }
    }

    if (!uvs.empty()) {
        result = AddBuffer(
            static_cast<const void *>(uvs.data()), 
            sizeof(float) * uvs.size(), 
            sizeof(float) * 2, 
            VK_FORMAT_R32G32_SFLOAT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (!result) {
            DuskLogError("Failed to create UV buffer");
            return false;
        }
    }

    if (!colors.empty()) {
        result = AddBuffer(
            static_cast<const void *>(colors.data()), 
            sizeof(float) * colors.size(), 
            sizeof(float) * 4, 
            VK_FORMAT_R32G32B32A32_SFLOAT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (!result) {
            DuskLogError("Failed to create color buffer");
            return false;
        }
    }

    if (!tangents.empty()) {
        result = AddBuffer(
            static_cast<const void *>(tangents.data()), 
            sizeof(float) * tangents.size(), 
            sizeof(float) * 4, 
            VK_FORMAT_R32G32B32A32_SFLOAT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (!result) {
            DuskLogError("Failed to create tangent buffer");
            return false;
        }
    }

    if (!bitangents.empty()) {
        result = AddBuffer(
            static_cast<const void *>(bitangents.data()), 
            sizeof(float) * bitangents.size(), 
            sizeof(float) * 4, 
            VK_FORMAT_R32G32B32A32_SFLOAT,
            VK_VERTEX_INPUT_RATE_VERTEX,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (!result) {
            DuskLogError("Failed to create bitangent buffer");
            return false;
        }
    }

    return true;
}

bool VulkanMesh::AddBuffer(const void * data, VkDeviceSize size, uint32_t stride, 
    VkFormat format, VkVertexInputRate inputRate, VkBufferUsageFlags usage)
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    _vkBindings.push_back({
        .binding = _nextBinding,
        .stride = stride,
        .inputRate = inputRate,
    });

    _vkAttributes.push_back({
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
    vkMapMemory(gfx->GetVkDevice(), memory, 0, size, 0, &ptr);
    memcpy(ptr, data, static_cast<size_t>(size));
    vkUnmapMemory(gfx->GetVkDevice(), memory);

    _vkBuffers.push_back(buffer);
    _vkBufferMemories.push_back(memory);

    ++_nextBinding;
    ++_nextLocation;

    return true;
}

void VulkanMesh::GenerateBindCommands(VkCommandBuffer vkCommandBuffer)
{
    // Must be >= the size of _vkBuffers
    VkDeviceSize offsets[] = { 0, 0, 0, 0, 0, 0, 0 };

    vkCmdBindVertexBuffers(vkCommandBuffer, 0, static_cast<uint32_t>(_vkBuffers.size()), _vkBuffers.data(), offsets);
}

void VulkanMesh::GenerateDrawCommands(VkCommandBuffer vkCommandBuffer)
{
    if (_indexed) {
        vkCmdDrawIndexed(vkCommandBuffer, _count, 1, 0, 0, 0);
    }
    else {
        vkCmdDraw(vkCommandBuffer, _count, 1, 0, 0);
    }
}

} // namespace Dusk::Vulkan